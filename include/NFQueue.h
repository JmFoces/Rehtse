/*
 * NFQueue.h
 *
 *  Created on: Jul 19, 2016
 *      Author: xshell
 */

#ifndef NFQUEUE_H_
#define NFQUEUE_H_
#include <iostream>
#include <list>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <misc.h>
#include <crafter.h>
#include <crafter/Utils/TCPConnection.h>
#include <flows/FlowTracker.h>
#include <boost/regex.hpp>
extern "C" {
  #include <linux/netfilter.h>  /* Defines verdicts (NF_ACCEPT, etc) */
  #include <libnetfilter_queue/libnetfilter_queue.h>
  #include <libnfnetlink/libnfnetlink.h>
  #include <libnetfilter_queue/libnetfilter_queue_ipv4.h>

struct nfq_data {
         struct nfattr **data;
 };

}
#include <boost/asio.hpp>
static int qpacket( struct nfq_q_handle *, struct nfgenmsg *, struct nfq_data *, void * );

class NFQueue {
	/**
	 * Abstracts interaction with netfilter queues.
	 */
public:
	boost::asio::io_service *ios;
	/**
	 * NFQueue()
	 * Perform necessary operations to register the queue.
	 * 	@param boost::asio::io_service*
	 */
	NFQueue(boost::asio::io_service*);
	/**
	 * Destroys the queue.
	 */
	virtual ~NFQueue();
	/**
	 * Dispatch the packet to the FlowTracker.
	 */
	void async_process_netfilterqueue_packet();
private:
	struct nfq_handle *nfqHandle = NULL;

    struct nfq_q_handle *myQueue = NULL;
	struct nfnl_handle *netlinkHandle = NULL;

	boost::asio::posix::stream_descriptor netfilterqueue_stream;

	int fd = -1;
	int res = -1;
 	char buf[4096] __attribute__ ((aligned));

};

#endif /* NFQUEUE_H_ */
