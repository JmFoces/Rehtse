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


#include <ostream>
#include <misc.h>
#include <boost/log/trivial.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>

std::string hexa_print(const unsigned char* array, int len){
	std::string result;
	std::stringstream stream;
	for (int i = 0; i < len;i++){
		stream << str(boost::format("%02x") % (unsigned short)array[i]);
	}
	result = stream.str();
	return result;
}

std::string hexa_print(const char* array, int len){

	return hexa_print((unsigned char*) array,len);
}


bool match_regex(std::string target, boost::regex regex){
	bool ret_value = false;
	boost::match_results<std::string::const_iterator> what;
	std::string::const_iterator start, end;
	boost::match_flag_type flags = boost::match_all;
	start = target.begin();
	end = target.end();
	if (boost::regex_search(start,end,what,regex,flags)){
		BOOST_LOG_TRIVIAL(debug) << "Packet Matches regex";
		ret_value = true;
	}
	return ret_value;
}
