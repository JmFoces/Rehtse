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


#include <flows/TCPFlow.h>

TCPFlow::TCPFlow(std::string key):Flow(key) {
	// TODO Auto-generated constructor stub

}

TCPFlow::~TCPFlow() {
	// TODO Auto-generated destructor stub
}
uint32_t TCPFlow::updateSeqNumber(uint32_t current_seq){
	return (current_seq + balance);
}
uint32_t TCPFlow::updateAckNumber(uint32_t current_ack){
	return (current_ack - balance);
}
bool TCPFlow::handlePacket(Crafter::Packet* packet){
	BOOST_LOG_TRIVIAL(trace) << "TCPFlow " << hexa_print(key.c_str(),key.size()) << " handling packet ";
	Crafter::TCP *transport_layer = (Crafter::TCP*)packet->operator [](1); /// TCP Will be placed always inside IP.
	int64_t balance_cache = 0;
	if (transport_layer->GetPSH()){
		BOOST_LOG_TRIVIAL(trace) << "Packet IS PUSH";
		Scanner *scanner = Scanner::instance();
		Pattern *pattern = scanner->check(packet);
		if (pattern){
			BOOST_LOG_TRIVIAL(trace) << "Packet matched filter " << pattern->print();
			// Apply modifications here!
			balance_cache += pattern->applyReplacement(packet->operator [](packet->GetLayerCount()-1));
			modified=true;
			BOOST_LOG_TRIVIAL(trace) << "Packet Updated " << balance;
		}else{
			BOOST_LOG_TRIVIAL(trace) << "No match";
		}
	}else{
		BOOST_LOG_TRIVIAL(trace) << "Packet IS NOT PUSH";
	}

	if (modified || ((TCPFlow*) brother)->modified){

		BOOST_LOG_TRIVIAL(debug) << "Keeping consistency of TCP Flows "
				<< transport_layer->GetSrcPort() << "->"
				<< transport_layer->GetDstPort();
		uint32_t current_seq = transport_layer->GetSeqNumber();
		BOOST_LOG_TRIVIAL(debug) << "Current SEQ " << current_seq;
		transport_layer->SetSeqNumber(updateSeqNumber(current_seq));
		BOOST_LOG_TRIVIAL(debug) << "MOD SEQ " << transport_layer->GetSeqNumber();
		uint32_t current_ack = transport_layer->GetAckNumber();
		BOOST_LOG_TRIVIAL(debug) << "Current ACK " << current_ack;
		transport_layer->SetAckNumber(
			((TCPFlow*)brother)->updateAckNumber(
				current_ack
			)
		);
		BOOST_LOG_TRIVIAL(debug) << "MOD ACK " << transport_layer->GetAckNumber();

		balance+=balance_cache;
		return true;
	}
	else{
		BOOST_LOG_TRIVIAL(trace) << "Touched anything";
		return false;
	}
}
void TCPFlow::updateKey(std::string &key_s,std::string &key_d, Crafter::IP* packet){
	BOOST_LOG_TRIVIAL(trace) << "Updating with TCP Key";
	Crafter::TCP *tcp_layer = (Crafter::TCP*) packet->GetTopLayer();
	uint16_t dport;
	dport = tcp_layer->GetDstPort();
	uint16_t sport;
	sport = tcp_layer->GetSrcPort();
	key_s.append((char*)&sport,2);
	key_d.append((char*)&dport,2);
	key_s.append((char*)&dport,2);
	key_d.append((char*)&sport,2);
}
