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
#include "UserInterface.h"


UserInterface::UserInterface() {
	init_logging(boost::log::trivial::trace);
}

UserInterface::~UserInterface() {
}

void UserInterface::init_logging(boost::log::trivial::severity_level severity)
{
    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= severity
    );
}


void UserInterface::start(){
	//Do necessary init functions here.
	Crafter::InitCrafter();
}
void UserInterface::quit(){
	//RELEASE everything here
	delete scanner;
	Crafter::CleanCrafter();
	exit(0);
}
void UserInterface::run(){
	start();
	std::fstream config("./config.json",std::ios_base::in);
	scanner = Scanner::instance();
	if (config.fail() != 0){
		std::cerr << "./config.json not found! Bye!" << std::endl;
		exit (EXIT_FAILURE);
	}else{
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(config, pt);

		boost::property_tree::ptree log_level = pt.get_child(CONF_KEY_LOGLEVEL);
		boost::log::trivial::severity_level requested_log_level = (boost::log::trivial::severity_level)std::stoi(log_level.data());
		BOOST_LOG_TRIVIAL(trace) << "Log Level set TO: " << requested_log_level;
		init_logging(requested_log_level);

		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child(CONF_KEY_PATTERNS))
		{
			scanner->add_pattern(new Pattern(v.second));
		}
	}
	NFQueue nf(&ios);
	nf.async_process_netfilterqueue_packet();
	ios.run();

}
