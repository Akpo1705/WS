/*
 * server.h
 *
 *  Created on: 17 июл. 2023 г.
 *      Author: root
 */

#ifndef NETWORKING_SERVER_H_
#define NETWORKING_SERVER_H_

#include <sys/socket.h>
#include <netinet/in.h>

struct Server{
	int domain;
	int service;
	int protocol;
	u_long interface;
	int port;
	int backlog;
	struct sockaddr_in address;
	struct sockaddr_in client;
	int socket;

};

struct Server server_constructor(unsigned int domain,unsigned int service,unsigned int protocol, u_long interface, int port, int backlog);
#endif /* NETWORKING_SERVER_H_ */
