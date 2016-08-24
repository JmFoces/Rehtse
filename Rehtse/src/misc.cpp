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

uint64_t char_to_mac(unsigned char *data){
	uint64_t result = ((uint64_t)data[0] << 40) |
					  ((uint64_t)data[1] << 32) |
					  ((uint64_t)data[2] << 24) |
					  ((uint64_t)data[3] << 16) |
					  ((uint64_t)data[4] << 8)  |
					  ((uint64_t)data[5] << 0);
	return result;
}
uint32_t char_to_ip(unsigned char *data){
	uint32_t result = ((uint32_t)data[0] << 24) |
					  ((uint32_t)data[1] << 16) |
					  ((uint32_t)data[2] << 8)  |
					  ((uint32_t)data[3] << 0);
		return result;
}
uint32_t int8_ptr_to_uint32(uint8_t *data){
	uint32_t result = ((uint32_t)data[3] << 24) |
					  ((uint32_t)data[2] << 16) |
					  ((uint32_t)data[1] << 8)  |
					  ((uint32_t)data[0] << 0);
	return result;
}
uint32_t int8_ptr_to_uint32_shift(uint8_t *data){
	uint32_t result = ((uint32_t)data[0] << 24) |
					  ((uint32_t)data[1] << 16) |
					  ((uint32_t)data[2] << 8)  |
					  ((uint32_t)data[3] << 0);
	return result;
}

ether_addr std_char_to_mac(unsigned char *data){
	ether_addr result;
	for(int i = 0; i < 6 ; i++){
		result.ether_addr_octet[i] = (u_int8_t)data[i];
	}

	return result;
}


void char_shift(unsigned char* array,uint8_t len,uint8_t* result){
	uint64_t shift = 0;
	for(int lendec = len-1;lendec>=0;lendec--){
		result[shift] = array[lendec];
		shift = shift + 1;
	}
}

boost::format format_mac_addr(uint8_t* mac_addr)
{

	return boost::format("%+02x:%+02x:%+02x:%+02x:%+02x:%+02x")
				% (unsigned short)mac_addr[0]
				% (unsigned short)mac_addr[1]
				% (unsigned short)mac_addr[2]
				% (unsigned short)mac_addr[3]
				% (unsigned short)mac_addr[4]
				% (unsigned short)mac_addr[5];
}
boost::format format_ip_addr(uint8_t* ip_addr)
{
	return boost::format("%+d.%+d.%+d.%+d")
	        	% (uint32_t)ip_addr[0]
				% (uint32_t)ip_addr[1]
			    % (uint32_t)ip_addr[2]
			    % (uint32_t)ip_addr[3];
}
boost::format format_ip_addr(char* ip_addr)
{
	return boost::format("%+d.%+d.%+d.%+d")
		        	% (uint32_t)ip_addr[0]
					% (uint32_t)ip_addr[1]
				    % (uint32_t)ip_addr[2]
				    % (uint32_t)ip_addr[3];
}


bool compare_arr(uint8_t* first,uint8_t *second,size_t size){
	for (size_t i = 0; i < size; i++){
		if (first[i]!=second[i])return false;
	}
	return true;
}


void call_ios(boost::asio::io_service* ios){
	try{
		ios->run();
	}catch(std::exception &e){
		BOOST_LOG_TRIVIAL(fatal) << "Exception raised " << e.what();
	}

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
