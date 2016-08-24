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

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_
#include <iostream>
#include <cstddef>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <crafter.h>
#include <radix_tree.hpp>
#include <flows/FlowTracker.h>
#include <NFQueue.h>
#include <pattern/Scanner.h>

class UserInterface {
	/*
	 * Class designed to interact with user.
	 */
public:
	static const char OP_CODE_SHOW='s';
	static const char OP_CODE_QUIT='q';
	/*
	 * UserInterface()
	 * 	Not much to say, initializes logger.
	 * 	In the future it will configure filename instead of using hardcoded config.json.
	 */
	UserInterface();
	virtual ~UserInterface();
	/*
	 * run()
	 * 	Parses config.json. Instantiates Scanner and add as patterns as defined in the file.
	 */
	void run();
	/*
	 * start()
	 * 	Initializes Crafter.
	 * 	In the future any necessary previous operations will be performed here.
	 */
	void start();
	/*
	 * quit()
	 * 	Releases all and exits
	 */
	void quit();
	/*
	 * Initializes logging without file, just to STDOUT.
	 */
	void init_logging(boost::log::trivial::severity_level );

private:
	boost::asio::io_service ios;
	Scanner* scanner;
};

#endif /* USERINTERFACE_H_ */
