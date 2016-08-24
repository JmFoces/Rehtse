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

#ifndef FLOWS_TCPFLOW_H_
#define FLOWS_TCPFLOW_H_

#include <flows/Flow.h>
#include <pattern/Scanner.h>

class TCPFlow: public Flow {
	/*
	 * Class representing a TCP Flow of information.
	 * It considers balance to update seq and ack numbers.
	 */
public:
	/*
	 * Generates keys considering first byte pair [0,1] as sport and
	 * the second one [2,3] as dport.
	 * Updates content of strings given as parameter.
	 */
	static void updateKey(std::string &key_s,std::string &key_d,Crafter::IP* packet);
	TCPFlow(std::string);
	virtual ~TCPFlow();
	/*
	 * If a packet matches it with a pattern it will update it.
	 * Handles seq and ack numbers to make everything traspartent.
	 * Once modify it will update further packets inside this connection.
	 * Use its brother to keep everything consistent.
	 */
	bool handlePacket(Crafter::Packet*);
	/*
	 * Updates Seq number
	 */
	uint32_t updateSeqNumber(uint32_t);
	/*
	 * Updates ACK number
	 */
	uint32_t updateAckNumber(uint32_t);
	inline uint32_t getBalance(){
		return balance;
	}

private:
	int64_t balance=0;
};

#endif /* FLOWS_TCPFLOW_H_ */
