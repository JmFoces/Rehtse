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

#include <flows/FlowTracker.h>
#include <flows/TCPFlow.h>

FlowTracker* FlowTracker::single_ptr=NULL;
FlowTracker* FlowTracker::instance() {
	if (FlowTracker::single_ptr == NULL) single_ptr = new FlowTracker();
	return single_ptr;
}


FlowTracker::FlowTracker() {


}

FlowTracker::~FlowTracker() {

}

void FlowTracker::traverse(){
    radix_tree<std::string, Flow*>::iterator it;

    BOOST_LOG_TRIVIAL(trace) << "traverse:" << std::endl;
    for (it = peer_tree.begin(); it != peer_tree.end(); ++it) {
    	BOOST_LOG_TRIVIAL(trace) << "    "
    			<< hexa_print((unsigned char*)it->first.c_str(),4)
				<< ", "
				<< it->second
				<< std::endl;
    }
}

bool FlowTracker::handlePacket(Crafter::Packet *packet){

	Crafter::IP* ip_layer = (Crafter::IP*)packet->operator [](0);
	BOOST_LOG_TRIVIAL(trace) << "Handling packet";
	std::pair<std::string,std::string> key_pair = buildFlowKey(
			ip_layer
	);
	Flow* packet_flow = getFlow(key_pair.first,key_pair.second);

	return packet_flow->handlePacket(packet);
}

Flow* FlowTracker::getFlow(std::string key,std::string brother_key){
	BOOST_LOG_TRIVIAL(debug) << "Flow Tracker access with key " << hexa_print(key.c_str(),key.size());
	radix_tree<std::string, Flow*>::iterator it;
	it = peer_tree.find(key);
	Flow* flow ;
	std::string found_key;
	if (peer_tree.end()!=it ){
		BOOST_LOG_TRIVIAL(debug) << "Flow Tracked already ";
		 found_key = it->first;
		 flow= it->second;
	}else{
		BOOST_LOG_TRIVIAL(debug) << "New Flow to track ";
		std::pair<Flow*,Flow*>* flowptr_pair = Flow::makeFlowPair((uint8_t)key[8],key,brother_key);
		peer_tree[key]=flowptr_pair->first;
		peer_tree[brother_key]=flowptr_pair->second;
		flow = flowptr_pair->first;
		delete flowptr_pair;
	}


	return flow;
}


std::pair<std::string,std::string> FlowTracker::buildFlowKey(Crafter::IP* packet){
	std::string key_s;
	std::string key_d;
	BOOST_LOG_TRIVIAL(trace) << "Building access keys";
	uint8_t* s_ip =  packet->GetRawSourceIP();
	uint8_t* d_ip =  packet->GetRawDestinationIP();

	key_s.append((char*)s_ip, 4);
	key_s.append((char*)d_ip, 4);

	key_d.append((char*)d_ip,4);
	key_d.append((char*)s_ip,4);

	uint8_t protocol = (char)packet->GetProtocol();
	key_s.append((char*) &protocol,1);
	key_d.append((char*) &protocol,1);

	switch ( protocol ) {
		case IPPROTO_TCP:
			TCPFlow::updateKey(key_s,key_d,packet);
			break;
		/*
		    case IPPROTO_ICMP:
			BOOST_LOG_TRIVIAL(trace) << "ICMP";
			Crafter::ICMP *icmp_layer;
			icmp_layer= (Crafter::ICMP*)packet->GetTopLayer();
			uint16_t icmp_id;
			icmp_id = icmp_layer->GetIdentifier();
			key_s.append((char*) &icmp_id,2);
			key_d.append((char*) &icmp_id,2);
			break;
		*/
		default:
			BOOST_LOG_TRIVIAL(info) << "unknown proto";
			break;
	}
	BOOST_LOG_TRIVIAL(trace) << "Got key_S " << hexa_print(key_s.c_str(),key_s.size());
	BOOST_LOG_TRIVIAL(trace) << "Got key_D " << hexa_print(key_d.c_str(),key_s.size());
	return std::pair<std::string,std::string>(key_s,key_d);
}
