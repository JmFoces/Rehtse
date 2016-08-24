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

#include <pattern/Scanner.h>
Scanner* Scanner::single_ptr = NULL;

Scanner* Scanner::instance(){
	if (Scanner::single_ptr == NULL) single_ptr = new Scanner();
	return single_ptr;
}

void Scanner::add_pattern(Pattern *pattern){
	patterns.push_back(pattern);
}

Pattern* Scanner::check(Crafter::Packet *packet){
	BOOST_LOG_TRIVIAL(trace) << "Scanner checking packet ";
	Pattern *ptr =NULL;
	for(std::list<Pattern*>::iterator it = patterns.begin(); it != patterns.end();it++){
		if ((*it)->check(packet))
		{
			ptr = *it;
			break;
		}
	}
	return ptr;
}


Scanner::Scanner() {
}

Scanner::~Scanner() {
	//while(!patterns.empty()) delete patterns.front(), patterns.pop_front();
	for(std::list<Pattern*>::iterator it = patterns.begin(); it != patterns.end();it++){
		delete *it;
	}
	patterns.clear();
}

