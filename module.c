#include "module.h"

void module_clear(struct SERVER_STATE *ss)
{
	memset(ss, 0, sizeof(*ss));
}

int module_start(struct SERVER_STATE *ss){

	INFO(NOTIFY, 0, "запуск "MODULE_INFO"...");
	INFO(IMPORTANT, CODE_COMMON_INFO_VERSION, "Версия %s %d", MODULE_VERSION, MODULE_RELEASE);
	INFO(NOTIFY, 0, "Интервал обработки событий: %d мс", ss->sc.loop_ms);

	ss->start_t = transport_get_local_time();
	ss->start_ts = transport_get_proc_time();
	ss->module_loop_time = 0;
	ss->queue = queue_create();
	int i;
    for (i = 0; i < MAX_FD; i++) {
        ss->client_socket[i].fd = 0;
        ss->client_socket[i].state = CONNECTING;
    }

//	ss->last_ts = transport_get_local_time();
//	ss->dir_ts = transport_get_local_time();
	INFO(NOTIFY, 0, MODULE_INFO" запущен");
	return 0;
}

int module_stop(struct SERVER_STATE *ss){
	INFO(NOTIFY, 0, "останов "MODULE_INFO"...");
	transport_suspend_point_receive(ss->tr, TRANSPORT_SUSPEND_FOREVER);
	return 0;
}

int module_listen_loop(struct SERVER_STATE* ss){

    if ((ss->new_socket = accept(ss->server.socket, (struct sockaddr *)&ss->server.address, (socklen_t*)&ss->addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //char* buffer = (char*)malloc(30000 * sizeof(char));
    char buffer[30000];
	read(ss->new_socket,buffer,30000);

	DEBUG(DEVEL,"%s",buffer);

//	queue_put(ss->queue, buffer);
    close(ss->fd[0]);

    /* Send "string" through the output side of pipe */
    write(ss->fd[1], buffer, (strlen(buffer)+1));

	//write(ss->new_socket, ss->hello, strlen(ss->hello));

	close(ss->new_socket);
	return 0;
}

int module_loop(struct SERVER_STATE* ss){

	int msec;

	ss->cur_ts = transport_get_proc_time();

	pto_update();
	if(ss->tr) transport_loop(ss->tr,ss->sc.loop_ms);
	else usleep(10000);

	ss->cur_ts = transport_get_local_time();
	msec = transport_get_difftime_msec(ss->module_loop_time,ss->cur_ts);

	if(msec > ss->sc.loop_ms){

		module_listen_loop(ss);

//	    if((ss->pid = fork()) == -1)
//	    {
//	            perror("fork");
//	            exit(1);
//	    }
//		if(ss->pid == 0){
//			module_listen_loop(ss);
////			close(ss->fd[0]);//Child process does not need this end of the pipe
////			DEBUG(DEVEL,"%s","send");
////			/* Send "string" through the output side of pipe */
////			write(ss->fd[1], string, (strlen(string)+1));
//			//exit(0);
//		}
//		else{
//			close(ss->fd[1]);
//
//			char buffer[30000];
//			int nbytes = read(ss->fd[0], buffer, sizeof(buffer));
//
//			if(nbytes > 0)
//				DEBUG(DEVEL,"module loop %s",buffer);
//			//DEBUG(DEVEL,"nbytes %d",nbytes);
//			//exit(0);
//		}
	}

	return 0;
}

void client_close(struct fds * client)
{
    close(client->fd);
    client->fd = 0;
    client->state = CONNECTING;
    memset(client->buffer,0,BUF_LEN);
    client->readedLength = 0;
}

int send_buff(int csocket, const uint8_t *buffer, size_t bufferSize)
{
    int written = send(csocket,(char*)buffer, bufferSize, 0);

    if (written == -1) {
        close(csocket);
        perror("send failed");
        return EXIT_FAILURE;
    }
    if (written != bufferSize) {
        close(csocket);
        perror("written not all bytes");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void client_handler(struct fds* client){
	int frameSize = BUF_LEN;
	client->readedLength = 0;

	if(!client->readedLength){
		client->readedLength = recv(client->fd,(char*)client->buffer,BUF_LEN,0);
	}
    //fprintf(stdout,"%s\n",(char*)client->buffer);

    if (!client->readedLength) {
    	client_close(client);
        perror("recv failed");
    }

    assert(client->readedLength <= BUF_LEN);

    switch(client->state)
    {
    	case CONNECTING: {
    		struct http_header hdr;
    		memset(&hdr,0,sizeof(hdr));
    		char* buffer = ws_handshake(&hdr, client->buffer, client->readedLength, &frameSize);
    		fprintf(stdout,"\n%s\n", buffer);
   			send_buff(client->fd, (const unsigned char *) buffer, frameSize);
   			free(buffer);

   			client->state = CLOSING;
   			break;
    	}
    		break;
    	case OPEN: {}
    		break;
    	case CLOSING: {
    		fprintf(stdout,"client connect closing\n");
    		client->state = CLOSED;
    	}
    		break;
    	case CLOSED: {
    		fprintf(stdout,"client connect closed\n");
    		client_close(client);
    	}
    		break;
    	default:
            printf("Unknown state!");
            exit(-1);
            break;
    }
}

int test_module_loop(struct SERVER_STATE* ss)
{
	int i,sd, activity;
	unsigned int addrlen;
	int max_sd;
	int new_socket;

	int client_sent_quit_message = 0;

    for (i = 0; i < MAX_FD; i++) {
        ss->client_socket[i].fd = 0;
        ss->client_socket[i].state = CONNECTING;
    }

	do{
		FD_ZERO(&ss->readfds);
		FD_SET(ss->server.socket,&ss->readfds);

		max_sd = ss->server.socket;

		for(i = 0; i < MAX_FD; i++){
			sd = ss->client_socket[i].fd;

			if(sd > 0){
				FD_SET(sd, &ss->readfds);
			}

			if(sd > max_sd)
				max_sd = sd;
		}

		activity = select(max_sd + 1, &ss->readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno!=EINTR)) {
            printf("select error");
        }

        if (FD_ISSET(ss->server.socket, &ss->readfds)){
        	struct sockaddr_in addr;
        	memset(&addr, 0, addrlen =  sizeof(addr));
        	if ((new_socket = accept(ss->server.socket, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0) {
        	    perror("accept");
        	    exit(EXIT_FAILURE);
        	}

            fprintf(stdout,"new connection, socket fd is %d, ip is: %s, port; %d \n", new_socket, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

            for(i =0 ; i < MAX_FD; i++){
            	if(ss->client_socket[i].fd == 0){
            		ss->client_socket[i].fd = new_socket;
            		fprintf(stdout,"Adding to list of socket as %d\n", i);
            		break;
            	}
            }
        }


        for(i = 0; i < MAX_FD; i++){
        	sd = ss->client_socket[i].fd;

        	if(FD_ISSET(sd,&ss->readfds)){
        		client_handler(&ss->client_socket[i]);
        		client_close(&ss->client_socket[i]);
        	}
        }
	}
	while(!client_sent_quit_message);

//	char line[1024];
//	while(fgets(line,sizeof(line),ss->test_fd)){
//		DEBUG(DEVEL,"%s",line);
//	}

	fclose(ss->test_fd);

	return 0;
}
