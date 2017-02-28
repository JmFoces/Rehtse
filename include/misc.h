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

#pragma once
#ifndef _MISC
#define _MISC

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>

const std::string CONF_KEY_LOGLEVEL="debuglevel";
const std::string CONF_KEY_PATTERNS="patterns";
const std::string CONF_KEY_MATCH="match";
const std::string CONF_KEY_MODIFY="modify";
const std::string CONF_KEY_REPLACEMENT="replacement";
const std::string CONF_KEY_REGEX="regex";
const std::string CONF_KEY_BPF="bpf";
static const uint16_t JUMBO_MTU = 9000;
typedef boost::asio::generic::raw_protocol raw_protocol_t;
typedef boost::asio::generic::basic_endpoint<raw_protocol_t> raw_endpoint_t;

/**
 * Formats the array to hexadecimal.
 * @param const unsigned char*
 * @param int length
 * @returns std::string containing hexadecimal representation of the array.
 */
std::string hexa_print(const unsigned char* array, int lenght);
/**
 * Formats the array to hexadecimal.
 * @param const  char*
 * @param int length
 * @returns std::string containing hexadecimal representation of the array.
 */
std::string hexa_print(const char* array, int lenght);
/**
 * Checks the given regex over the target and returns the result.
 * @param std::string target content to match against regex
 * @param boost::regex regex regex to apply
 * @returns true if matches.
 */
bool match_regex(std::string target, boost::regex regex);
#endif
