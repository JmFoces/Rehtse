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
#ifndef FLOWS_FLOWTRACKER_H_
#define FLOWS_FLOWTRACKER_H_
#include <crafter.h>
#include <radix_tree.hpp>
#include <flows/Flow.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <misc.h>

class FlowTracker {
	/**
	 * Class that manages flow creation and access each time
	 * a packet traverses this program.
	 */
public:
	friend class Flow;
	/**
	 * Singleton stuff
	 * @return the unique instance of this class.
	 */
	static FlowTracker* instance();
	/**
	 * FixME: I'm not releasing memory.
	 */
	virtual ~FlowTracker();
	/**
	 * Defines the execution flow for each packet:
	 *	 1º Create access key.
	 *	 2º getFlow
	 *	 3º flow->handlePacket...
	 * @return bool. true if packet was modified at any step.
	 */
	bool handlePacket(Crafter::Packet *);
	/**
	 * Writes the radix tree to the console.
	 */
	void traverse();
	/**
	 * Look inside the radix for the first key.
	 * 		If finds it returns the given Flow.
	 * 		Else creates both flow and its brother and links them.
	 * @return 	Flow* contained in the radix
	 */
	Flow* getFlow(std::string, std::string);
	/**
	 * Creates a pair of strings to be used as keys to access the radix
	 * looking for a Flow(first) or its brother(second)
	 * @param Crafter::IP * ip layer to build the flow key from.
	 * @returns std::pair<std::string,std::string>
	 */
	std::pair<std::string,std::string> buildFlowKey(Crafter::IP* );
private:
	static FlowTracker* single_ptr;
	FlowTracker();
	radix_tree<std::string,Flow*> peer_tree;
};


#endif /* FLOWS_FLOWTRACKER_H_ */
