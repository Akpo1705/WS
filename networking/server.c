/*
 * server.c
 *
 *  Created on: 17 июл. 2023 г.
 *      Author: root
 */

#include "../module.h"

void print_server_state_addr(struct SERVER_STATE* ss){
	char servaddr[255];
	printf("Client Adress = %s",inet_ntop(AF_INET,&ss->server.address.sin_addr, servaddr,sizeof(servaddr)));
}

void print_server_addr(struct in_addr* sin_addr){
	char servaddr[255];
	printf("Adress = %s",inet_ntop(AF_INET,&sin_addr, servaddr,sizeof(servaddr)));
}

void server_init(struct SERVER_STATE* ss){
	int opt = TRUE;

	ss->hello = "HTTP/1.1 200 OK\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-type: text/html\nConnection: Closed\n\n<html><body><h1>Hello, world!!!</h1></body></html>";

	if((ss->server.socket = socket(AF_INET, SOCK_STREAM,0)) == 0){
		perror ("socket failed");
		exit(EXIT_FAILURE);
	}

    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(ss->server.socket , SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT , (char *)&opt, sizeof(opt)) < 0 ) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    ss->server.address.sin_family = AF_INET;
    ss->server.address.sin_addr.s_addr = INADDR_ANY;
    ss->server.address.sin_port = htons(ss->sc.port);

    if (bind(ss->server.socket, (struct sockaddr *)&ss->server.address, sizeof(ss->server.address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(ss->server.socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

struct Server server_constructor(unsigned int domain,unsigned int service,unsigned int protocol, u_long interface, int port, int backlog)
{
    struct Server server;
    // Define the basic parameters of the server.
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;
    bzero((char*)&server.address,sizeof(server.address));
    // Use the aforementioned parameters to construct the server's address.
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    //print_server_addr(server.address.sin_addr.s_addr);
    // Create a socket for the server.
    server.socket = socket(domain, service, protocol);

    int opt = TRUE;

    // Confirm the connection was successful.
    if (server.socket == 0)
    {
        perror("Failed to connect socket...\n");
        exit(1);
    }

    if(setsockopt(server.socket,SOL_SOCKET,SO_REUSEADDR,(char*)&opt, sizeof(opt))<0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Attempt to bind the socket to the network.
    if ((bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address))) < 0)
    {
        perror("Failed to bind socket...\n");
        exit(1);
    }
    // Start listening on the network.
    if ((listen(server.socket, server.backlog)) < 0)
    {
        perror("Failed to start listening...\n");
        exit(1);
    }
    return server;
}

