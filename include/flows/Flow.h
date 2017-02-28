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
#ifndef FLOWS_FLOW_H_
#define FLOWS_FLOW_H_

#include <arpa/inet.h>
#include <crafter.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <misc.h>
#include <boost/regex.hpp>


class Flow {
	/**
	 * Class that representing a communication channel.
	 */
public:
	/**
	 * FixME: this should not be here. Maybe... It should not know about sibling classes ??
	 * This is a factory method that creates both flow pairs
	 * for two generated keys.
	 */
	static std::pair<Flow*,Flow*>* makeFlowPair(uint8_t ,std::string,std::string);
	/**
	 * FixME: this should not be here.Maybe... It should not know about sibling classes ??
	 * This is a factory method that creates a flow a generated key.
	 */
	static Flow* makeFlow(uint8_t ,std::string);
	virtual ~Flow();
	/**
	 * Interface to be implemented by sibling classes.
	 */
	virtual bool handlePacket(Crafter::Packet*)=0;
	inline const Flow* getBrother() const {
			return brother;
	}
	inline void setBrother(Flow* brother) {
		this->brother = brother;
	}

protected:
	/**
	 * Creates a flow with the given key
	 * @param std::string key.
	 */
	Flow(std::string key);
	std::string key;
	Flow* brother;
	bool modified = false;

};

#endif /* FLOWS_FLOW_H_ */
