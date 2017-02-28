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

#include <flows/GenericFlow.h>

GenericFlow::GenericFlow(std::string key):Flow(key) {
}

GenericFlow::~GenericFlow() {
}

bool GenericFlow::handlePacket(Crafter::Packet *packet){
	BOOST_LOG_TRIVIAL(info) << "GenericFlow " << " handling packet ";
	BOOST_LOG_TRIVIAL(debug) << "GenericFlow " << hexa_print(key.c_str(),key.size());
	int32_t balance_cache = 0;
	Scanner *scanner = Scanner::instance();
	Pattern *pattern = scanner->check(packet);
	if (pattern){
		// Apply modifications here!
		balance_cache += pattern->applyReplacement(packet->operator [](packet->GetLayerCount()-1));
		modified=true;
	}else{
		BOOST_LOG_TRIVIAL(trace) << "No match";
	}

	if (modified){
		BOOST_LOG_TRIVIAL(info) << "GenericFlow " << "Updated Packet Content ";
		return true;
	}
	else{
		BOOST_LOG_TRIVIAL(info) << "Touched anything";
		return false;
	}
}
void GenericFlow::updateKey(std::string &key_s,std::string &key_d, Crafter::IP* packet){
	/*
	 * Care with this. It will work only when first 8 bytes of the packet are the ports.
	 * Otherwise it will make weird things. For protocols like ICMP for example.
	 * It generates good keys for TCP UDP and SCTP
	 */
	BOOST_LOG_TRIVIAL(debug) << "Updating with Generic Key";
	Crafter::Layer *ip_overlay = (Crafter::Layer*) packet->GetTopLayer();
	uint8_t *raw_data_ptr = (uint8_t *) malloc(ip_overlay->GetSize());
	ip_overlay->GetData(raw_data_ptr);
	uint16_t sport = raw_data_ptr[0];
	uint16_t dport = raw_data_ptr[2];
	key_s.append((char*)&sport,2);
	key_d.append((char*)&dport,2);
	key_s.append((char*)&dport,2);
	key_d.append((char*)&sport,2);
	delete raw_data_ptr;
}
