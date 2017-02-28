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
DISCLAIMED. IN NO EVENT SHALL JOSÉ MARÍA FOCES VIVANCOS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <flows/Flow.h>
#include <flows/TCPFlow.h>
#include <flows/GenericFlow.h>
Flow::Flow(std::string key) {
	this->brother=NULL;
	this->key = key;
}

Flow::~Flow() {

}

std::pair<Flow*,Flow*>* Flow::makeFlowPair(uint8_t protocol_id, std::string key0,std::string key1){
	std::pair<Flow*,Flow*>* ret_value = new std::pair<Flow*,Flow*>(NULL,NULL);
	switch(protocol_id){
		case IPPROTO_TCP:
			ret_value->first= (Flow*) new TCPFlow(key0);
			ret_value->second= (Flow*) new TCPFlow(key1);
			break;
		default:
			ret_value->first= (Flow*) new GenericFlow(key0);
			ret_value->second= (Flow*) new GenericFlow(key1);
			break;
	}
	ret_value->first->setBrother(ret_value->second);
	ret_value->second->setBrother(ret_value->first);
	return ret_value;
}


Flow* Flow::makeFlow(uint8_t protocol_id, std::string key){
	Flow *ret_value=NULL;
	switch(protocol_id){
		case IPPROTO_TCP:
			BOOST_LOG_TRIVIAL(error) << " Not possible to create a single TCPFlow. Use makeFlowPair method";
			ret_value = NULL;
			break;
		default:
			ret_value = (Flow*)new GenericFlow(key);
			break;
	}
	return ret_value;
}
