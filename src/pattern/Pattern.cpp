/*
Copyright (c) 2016, José María Foces Vivancos
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ESTEBAN PELLEGRINO BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <pattern/Pattern.h>

Pattern::Pattern(std::string bpf,std::string regex,std::string replacement_regex, std::string raw_replacement):
	bpf_string(bpf),
	regex(regex),
	replacement_regex(replacement_regex),
	raw_replacement(raw_replacement){

	init_pcap_bpf();
}
void Pattern::init_pcap_bpf(){
	//pcap = pcap_open_dead(DLT_EN10MB,65535);//Allocs mem.
	pcap = pcap_open_dead(DLT_RAW,65535);//Allocs mem.
	bpf_compiled = (struct bpf_program* ) malloc(sizeof(bpf_program));
	if (pcap_compile(pcap,bpf_compiled,bpf_string.c_str(),0,PCAP_NETMASK_UNKNOWN) < 0 ){
		BOOST_LOG_TRIVIAL(error) << "Error compiling " << pcap_geterr(pcap);
		BOOST_LOG_TRIVIAL(error) << "Cannot compile BPF "<< bpf_string;
		throw BPF_ERR;
	}

}
Pattern::Pattern(boost::property_tree::ptree ptree) {
	boost::property_tree::ptree match = ptree.get_child(CONF_KEY_MATCH);
	boost::property_tree::ptree replacement = ptree.get_child(CONF_KEY_REPLACEMENT);
	bpf_string = match.get_child(CONF_KEY_BPF).data();
	regex = boost::regex(match.get_child(CONF_KEY_REGEX).data());
	replacement_regex =	boost::regex(replacement.get_child(CONF_KEY_REGEX).data());
	raw_replacement = replacement.get_child(CONF_KEY_REPLACEMENT).data();
	init_pcap_bpf();
}
Pattern::~Pattern() {
	delete bpf_compiled;
	pcap_close(pcap);//Releases mem.
}


bool Pattern::check(Crafter::Packet *packet){
	BOOST_LOG_TRIVIAL(debug) << "Pattern" << print() << "checking packet..." ;
	bool ret_value = false;
	size_t size = packet->GetSize();
	uint8_t* raw_packet= (uint8_t*) malloc(
		size
	);
	size = packet->GetData(raw_packet);
	BOOST_LOG_TRIVIAL(trace) << "PACKET DATA" <<  hexa_print((unsigned char* ) raw_packet, size);
	/*uint32_t bpf_exec_result = bpf_filter( //if 0 then  matches
		bpf_compiled->bf_insns,
		(unsigned char*) raw_packet,
		size,
		size
	);*/
	pcap_pkthdr pkt_hdr;
	pkt_hdr.caplen = size;
	pkt_hdr.len = size;
	int bpf_exec_result = pcap_offline_filter(
		bpf_compiled,
		&pkt_hdr,
		raw_packet
	);

	BOOST_LOG_TRIVIAL(trace) << "BPF Program exit code: " << bpf_exec_result;
	std::string raw_pack_str((const char*)raw_packet,size);
	if(bpf_exec_result != 0){
		//FIXME
		BOOST_LOG_TRIVIAL(debug) << "BPF Match "<< bpf_string;
		ret_value = match_regex(raw_pack_str,regex);
	}
	if(ret_value)BOOST_LOG_TRIVIAL(info) << "Pattern" << print() << "Matched!";
	return ret_value;
}

int32_t Pattern::applyReplacement(Crafter::Layer *layer){
	BOOST_LOG_TRIVIAL(debug) << "Pattern" << print() << "Updating packet contents..." ;
	std::string payload = layer->GetStringPayload();
	std::ostringstream t(std::ios::out | std::ios::binary);
	std::ostream_iterator<char, char> oi(t);
	BOOST_LOG_TRIVIAL(trace) << "BASE PAYLOAD " << hexa_print(payload.c_str(),payload.size());
	//boost::regex_replace(oi, payload.begin(),payload.end(),regex, format, boost::match_default | boost::format_all);
	std::string payload_mod =  boost::regex_replace(
		payload,
		replacement_regex,
		raw_replacement,
		boost::match_default | boost::format_first_only
	);

	layer->SetPayload((unsigned char*)payload_mod.c_str(),payload_mod.size());
	BOOST_LOG_TRIVIAL(trace) << "Payload mod PAYLOAD " << hexa_print(layer->GetStringPayload().c_str(),layer->GetStringPayload().size());

	if (payload != payload_mod) BOOST_LOG_TRIVIAL(info) << "Pattern" << print() << "Updated packet content" ;
	return (payload_mod.size() - payload.size());
}
