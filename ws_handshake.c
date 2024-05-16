#include "module.h"
#include "ws_file.h"

static int http_parse_headers(struct http_header *header, const char *hdr_line)
{
    char *p;
    const char * header_name = hdr_line;
    char * header_content = p = strchr(header_name, ':');

    if (!p) {
        return -1;
    }

    *p = '\0';
    do {
        header_content++;
    } while (*header_content == ' ');

    if (!strcmp(WS_HDR_UPG, header_name)) {
        header->upgrade = !strcmp(WS_WEBSOCK, header_content);
        return 0;
    }

    if(!strcmp(WS_HDR_HST, header_name)){
    	memcpy(&header->host,header_name,sizeof(header->host));
    }

    if (!strcmp(WS_HDR_VER, header_name)) {
        header->version = atoi(header_content);
        return 0;
    }

    if (!strcmp(WS_HDR_KEY, header_name)) {
        memcpy(&header->key, header_content, sizeof(header->key));
        return 0;
    }

    if(!strcmp(WS_HDR_MOBILE, header_name)){
    	header->mobile = !strcmp(WS_MOBILE, header_content);
    	return 0;
    }

    if(!strcmp(WS_HDR_CON, "Connection")){
    	if(!strcmp(header_content, "keep-alive"))
    		header->state = CONNECTING;
    	else
    	    header->state = KEEP_ALIVE;
    }

    if(!strcmp(WS_HDR_UAG, header_name)){
    	memcpy(&header->user_agent,header_content, sizeof(header->user_agent));
    	return 0;
    }

    if(!strcmp(WS_HDR_ACCEPT_LANGUAGE, header_name)){
    	memcpy(&header->accept_lang, header_content, sizeof(header->accept_lang));
    	return 0;
    }

    if(!strcmp(WS_HDR_ACC_ENC, header_name)){
    	memcpy(&header->accept_encoding, header_content ,sizeof(header->accept_encoding));
    	return 0;
    }

    if(!strcmp(WS_HDR_ACC, header_name)){
    	memcpy(&header->accept, header_content, sizeof(header->accept));
    }

    if(!strcmp(WS_HDR_FETCH_DEST, header_name)){
    	if(strcmp(header_content,"image") == 0){
    		header->fetchDest = IMAGE;
    	}else if(strcmp(header_content,"style") == 0){
    		header->fetchDest = STYLE;
    	}else if(strcmp(header_content,"document") == 0){
    		header->fetchDest = DOCUMENT;
    	}
    }

    //if(!strcmp())

    *p = ':';
    return 0;
}

static int http_parse_request_line(struct http_header *header, const char* hdr_line){

	char* line = (char*)hdr_line;
	char *token = strtok(line, " ");

	if(token){
		memcpy(&header->method, token, sizeof(header->method));
		if(strcmp(token,"GET")){
			header->wsMethod = GET;
		}
		if(strcmp(token,"POST")){
			header->wsMethod = POST;
		}
		if(strcmp(token,"PUT")){
			header->wsMethod = PUT;
		}
	}

	token = strtok(NULL, " ");

	if(token){
		memcpy(&header->uri, token, sizeof(header->uri));
	}

	return 0;
}

static int http_header_readline(char *in, char *buf, int len)
{
    char *ptr = buf;
    char *ptr_end = ptr + len - 1;

    while (ptr < ptr_end) {
        *ptr = *in++;
        if (*ptr != '\0') {
            if (*ptr == '\r') {
                continue;
            } else if (*ptr == '\n') {
                *ptr = '\0';
                //fprintf(stdout,"buf 0x%p ptr 0x%p, ptr_end 0x%p \n",buf, ptr, ptr_end);
                return ptr - buf;
            } else {
                ptr++;
                continue;
            }
        } else {
            *ptr = '\0';
            //fprintf(stdout,"buf 0x%p ptr 0x%p, ptr_end 0x%p \n",buf, ptr, ptr_end);
            return ptr - buf;
        }
    }
    return 0;
}

static void ws_http_parse_handsake_header(struct http_header * header, uint8_t *in_buf, int in_len){

	char* header_line = (char*)in_buf;
	int res, count = 0;

	header->type = WS_ERROR_FRAME;

	while((res = http_header_readline((char*)in_buf, header_line, in_len - 2)) > 0){

		if(!count){
			http_parse_request_line(header, header_line);
		}
		else
		{
			http_parse_headers(header, header_line);
		}

		count ++;
		in_buf += res + 2;
	}
    //fprintf(stdout,"key %s version %d \n\n", header->key, header->version);
	if (header->key && header->version == WS_VERSION) {
		header->type = WS_OPENING_FRAME;
	}
}

char* getTimeString() {
    time_t now;
    time(&now);
    return ctime(&now);
}

static int ws_get_handshake_header_basic(char* out_buff, int out_len){
	int written = 0;
	char timeBuf[100];
	getTimeString(timeBuf);
	written = (snprintf((char*) out_buff, out_len,
			"HTTP/1.1 200 OK\r\n"
			"%s: %s\r\n"
			"%s: %s\r\n"
			"%s: %s\r\n\r\n",
			WS_HDR_COT,WS_HDR_TEXT_HTML,
			WS_HDR_CON,WS_HDR_CON_KEEP_ALIVE,
			WS_HDR_DATE,timeBuf
	));
	return written;
}

char* ws_handshake(struct http_header *header, uint8_t *in_buf, int in_len, int *out_len){

	char out_header[512];
	unsigned char timeBuf[100];
	size_t wr;

	ws_http_parse_handsake_header(header, in_buf, in_len);

//	if(header->fetchDest == STYLE){
//		getTimeString(timeBuf);
//			wr = snprintf((char *)out_header, sizeof(out_header),
//					"HTTP/1.1 200 OK\r\n"
//					"%s: %s\r\n"
//					"%s: %s\r\n\r\n",
//					WS_HDR_COT,WS_HDR_TEXT_CSS,
//					WS_HDR_DATE,""
//			);
//			return gethtmlfile("./styles/styles.css",out_header,wr, out_len);
//	}
	if(!strcmp(header->method,"GET") && !strcmp(header->uri,"/")){
		wr = ws_get_handshake_header_basic(out_header,sizeof(out_header));
		return gethtmlfile("./html/index.html", out_header, wr ,out_len);
	}
	else if (!strcmp(header->method,"GET") && !strcmp(header->uri,"/favicon.ico")){

			getTimeString(timeBuf);
			wr = snprintf((char *)out_header, sizeof(out_header),
					"HTTP/1.1 200 OK\r\n"
					"%s: %s\r\n"
					"%s: %s\r\n\r\n",
					WS_HDR_COT,WS_HDR_TEXT_HTML,
					WS_HDR_DATE,timeBuf
			);
			return gethtmlfile("./images/favicon.ico",out_header,wr, out_len);
	}
	else if(!strcmp(header->method,"GET")&& !strcmp(header->uri,"/styles.css")){
		getTimeString(timeBuf);
		wr = snprintf((char *)out_header, sizeof(out_header),
				"HTTP/1.1 200 OK\r\n"
				"%s: %s\r\n"
				"%s: %s\r\n\r\n",
				WS_HDR_COT,WS_HDR_TEXT_CSS,
				WS_HDR_DATE,timeBuf
		);
		return gethtmlfile("./styles/styles.css",out_header,wr, out_len);
	}
	else{
		parse_uri(header->uri);
		fprintf(stdout,"\n\n\n\n404 request\n");
		wr = snprintf((char *)out_header, sizeof(out_header),
		                           "HTTP/1.1 400 Bad Request\r\n"
		                           //"%s: %d\r\n\r\n"
									"%s: %s\r\n\r\n",
		                           //"Bad request",
		                           //WS_HDR_VER, WS_VERSION,
								   WS_HDR_COT,WS_HDR_TEXT_HTML);
		return gethtmlfile("./html/404.html", out_header, wr, out_len);
	}

	return NULL;
}
