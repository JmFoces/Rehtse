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
#ifndef MATCH_PATTERN_H_
#define MATCH_PATTERN_H_
#include <iostream>
#include <string>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/regex.hpp>
#include <crafter.h>
#include <pcap.h>
#include <misc.h>
#include <err.h>
class Pattern {
	/**
	 * Class that implements a Pattern. A pattern is composed by:
	 * 		BPF filter
	 *	 	Regex
	 * 	Plus some replacement stuff that fits the operations for what this
	 * 	program is thought to be.
	 *		Replacement is composed by a regex and a replacement string.
	 */
public:
	/**
	 * Constructs a pattern from:
	 *	 	bpf_string(bpf),
	 *		regex(regex),
	 *		replacement_regex(replacement_regex),
	 *		raw_replacement(raw_replacement)
	 * It compiles the bpf program and regex.
	 * Raises:
	 * 		BPF_ERR if cannot compile the filter.
	 * @param std::string bpf filter string.
	 * @param std::string regex to match packets against
	 * @param std::string regex to replace content
	 * @param std::string replacement string
	 */
	Pattern(std::string,std::string,std::string,std::string );
	/**
	 * Constructs a pattern from ptree (directly from config file).
	 * It compiles the bpf program and regex.
	 * Raises:
	 * 		BPF_ERR if cannot compile the filter.
	 * @param boost::property_tree::ptree to create the object from.
	 */
	Pattern(boost::property_tree::ptree);
	/**
	 * Checks if the packet matches the given BPF program and regex.
	 * @param Crafter::Packet * to be checked for matching this pattern.
	 */
	bool check(Crafter::Packet *);
	//bool check(unsigned char *);
	virtual ~Pattern();
	/**
	 * Writes printable parts of itself to a string.
	 */
	std::string print(){
		std::stringstream ss;
		ss << "BPF: ";
		ss << bpf_string;
		ss << " REGEX: ";
		ss << regex.str();
		return ss.str();
	}
	/**
	 * Apply replacement regex over the given layer.
	 * @param Crafter::Layer * over what the replacement will be applied
	 * @return int32_t meaning the bytes balance: >0 if added data <0
	 * if replaced data is smaller than the provided one
	 */
	int32_t applyReplacement(Crafter::Layer *);
private:
	/**
	 * Compiles the BPF program.
	 */
	void init_pcap_bpf();
	std::string bpf_string;
	boost::regex regex;
	bpf_program *bpf_compiled=NULL;
	pcap_t *pcap;

	boost::regex replacement_regex;
	std::string  raw_replacement;

};

#endif /* MATCH_PATTERN_H_ */
