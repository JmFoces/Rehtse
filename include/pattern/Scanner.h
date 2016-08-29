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

#ifndef PATTERN_SCANNER_H_
#define PATTERN_SCANNER_H_
#include <cstddef>
#include <pattern/Pattern.h>
class Scanner {
	/**
	 * Class to perform operations with a set of patterns.
 	 *	  Singleton.
	 * It's thought to be a manager for this set of patterns allowing to
	 * perform dynamic operations over it create/destroy/modify patterns
	 * live.
	 * The main function now is to check if a packet matches with any pattern
	 * in the set.
	 */
public:
	/**
	 * Singleton stuff.
	 * @return the unique instance of this class.
	 */
	static Scanner* instance();
	/**
	 * Releases all patterns in the list, and clean the list.
	 */
	virtual ~Scanner();
	/**
	 * Adds a pattern at the end of the list.
	 * @param Pattern* to check packets against.
	 */
	void add_pattern(Pattern*);
	/**
	 * Check if packet matches with any of the patterns contained in the list.
	 * If any patterns matches it stops searching and returns this pattern.
	 * The patterns are evaluated considering insertion order.
	 * @param Crafter::Packet* to be checked. It modifies if packet matches any pattern.
	 */
	Pattern* check(Crafter::Packet *);
private:
	Scanner();
	static Scanner* single_ptr ;
	std::list<Pattern*> patterns;
};

#endif /* PATTERN_SCANNER_H_ */
