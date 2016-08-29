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

#ifndef FLOWS_GENERICFLOW_H_
#define FLOWS_GENERICFLOW_H_


#include <flows/Flow.h>
#include <pattern/Scanner.h>
#include <pattern/Pattern.h>
class GenericFlow: public Flow {
	/**
	 * Class representing a GenericFlow of information.
	 * It does not consider any facts related to connection oriented protocols.
	 */
public:
	/**
	 * Generates keys considering first byte pair [0,1] as sport and
	 * the second one [2,3] as dport.
	 * Updates content of strings given as parameter.
	 * @param std::string & key source direct multiplexing key
	 * @param std::string & key destination opposite multiplexing key
	 * @param Crafter::IP* packet packet to take data from
	 */
	static void updateKey(std::string &key_s,std::string &key_d,Crafter::IP* packet);
	/**
	 * Creates an instance from the key provided.
	 * @param std::string key identifying this flow.
	 */
	GenericFlow(std::string);
	virtual ~GenericFlow();
	/**
	 * Implements the most generic modification flow.
	 * If matches, then change and dispatch.
	 * FixME:¿¿¿The flow is marked as modified and further packets will be crafted again
	 * anyway. ???
	 * @param Crafter::Packet* to be handled. This object may be modified.
	 * @return bool. True if packet was modified.
	 */
	bool handlePacket(Crafter::Packet*);

};

#endif /* FLOWS_GENERICFLOW_H_ */
