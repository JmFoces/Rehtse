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

#include "NFQueue.h"


NFQueue::NFQueue(boost::asio::io_service* ioservice):
	netfilterqueue_stream(*ioservice)
{
	// TODO Auto-generated constructor stub
	  this->ios=ios;
	 // Get a queue connection handle from the module
	  if (!(nfqHandle = nfq_open())) {
	    std::cerr << "Error in nfq_open()" << std::endl;
	    exit(-1);
	  }
	  if (nfq_unbind_pf(nfqHandle, PF_PACKET) < 0) {
	      std::cerr << "Error in nfq_unbind_pf()" << std::endl;
	      exit(1);
	    }
	  // Bind this handler to process IP packets...
	  if (nfq_bind_pf(nfqHandle, PF_PACKET) < 0) {
		std::cerr << "Error in nfq_bind_pf()" << std::endl;
		exit(1);
	  }
	  // Install a callback on queue 0
	  if (!(myQueue = nfq_create_queue(nfqHandle,  0, &qpacket, NULL))) {
		  std::cerr << "Error in nfq_create_queue()" << std::endl;
		  exit(1);
	  }
	  // Turn on packet copy mode
	  if (nfq_set_mode(myQueue, NFQNL_COPY_PACKET, 0xffff) < 0) {
		  std::cerr << "Could not set packet copy mode" << std::endl;
	      exit(1);
	  }
	  netlinkHandle = nfq_nfnlh(nfqHandle);
	  fd = nfnl_fd(netlinkHandle);
	  boost::system::error_code ec;
	  netfilterqueue_stream.assign( fd , ec );

	  BOOST_LOG_TRIVIAL(info) << "netfilter queue opened up as fd " << fd << " "<< ec.message() ;

}

NFQueue::~NFQueue() {
	// TODO Auto-generated destructor stub
}
void NFQueue::async_process_netfilterqueue_packet( )
{


	boost::asio::async_read(netfilterqueue_stream,
			boost::asio::mutable_buffers_1(buf,JUMBO_MTU),
			boost::asio::transfer_at_least(1),
			[&](const boost::system::error_code &error, std::size_t bytes_read)
			{
				if ( error )
				{
					BOOST_LOG_TRIVIAL(error) << "failed to read from netfilter queue (" << bytes_read << " bytes): " << error.message();
					return;
				}
				else
				{
					BOOST_LOG_TRIVIAL(trace) << "------------------------------------------------------------";
					BOOST_LOG_TRIVIAL(trace) << "read " << bytes_read << " bytes from netfilter queue : " <<  hexa_print(buf,bytes_read);

					// send the packet to the callback to be processed
					nfq_handle_packet( nfqHandle, (char*)buf, bytes_read );
				}


				// schedule another read
				/*int x;
				std::cin >> x;*/
				async_process_netfilterqueue_packet( );
			} );
	return;
}
static int qpacket(nfq_q_handle *myQueue, struct nfgenmsg *msg,
                    nfq_data *nfq_data, void *cbData) {
	uint32_t id = 0;
	FlowTracker* flow_tracker = FlowTracker::instance();
	nfqnl_msg_packet_hdr *header;

	if ((header = nfq_get_msg_packet_hdr(nfq_data))) {
		id = ntohl(header->packet_id);
	}

	unsigned char *pktData;
	int len = nfq_get_payload(nfq_data, &pktData);


	//Decoding
	BOOST_LOG_TRIVIAL(trace) <<  "Packet: "<< hexa_print((unsigned char *)pktData,len);
	Crafter::Packet packet_obj((uint8_t*)pktData,len,Crafter::IP::PROTO);
	std::stringstream ss;
	packet_obj.Print(ss);
	BOOST_LOG_TRIVIAL(trace) << "DECODED Packet "<< ss.str();

	bool packet_modified = flow_tracker->handlePacket(&packet_obj);
	if (packet_modified){
		//Recraft-> dispatch
		packet_obj.PreCraft();
		int length_mod = packet_obj.GetData(pktData);
		ss = std::stringstream();
		packet_obj.Print(ss);
		BOOST_LOG_TRIVIAL(trace) << "DECODED MODIFIED Packet "<< ss.str();
		BOOST_LOG_TRIVIAL(trace) << "PACKET MOD " << hexa_print(pktData,length_mod);
		return nfq_set_verdict(myQueue, id, NF_ACCEPT, length_mod,pktData);
	}else{
		//Send as it came to us.
		return nfq_set_verdict(myQueue, id, NF_ACCEPT, len,pktData);
	}
}
