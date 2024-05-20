/*
 * HTTPRequest.h
 *
 *  Created on: 19 июл. 2023 г.
 *      Author: root
 */

#ifndef NETWORKING_HTTPREQUEST_H_
#define NETWORKING_HTTPREQUEST_H_

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "../datastructure/Dictionary.h"
#include "../datastructure/Entery.h"
#include "../datastructure/BinarySearchTree.h"
#include "../datastructure/queue.h"
#include "../datastructure/LinkedList.h"

#define PRINT(e) fprintf(stdout,"%s\n",e)

struct HTTPRequest{
	struct Dictionary request_line;
	struct Dictionary header_fields;
	struct Dictionary body;
};

struct HTTPRequest http_request_constructor(char* request_string);
void http_request_destructor(struct HTTPRequest *request);
// Functions for extracting the constituent elements from a request string.
void extract_request_line_fields(struct HTTPRequest *request, char *request_line);
void extract_header_fields(struct HTTPRequest *request, char *header_fields);
void extract_body(struct HTTPRequest *request, char *body);
#endif /* NETWORKING_HTTPREQUEST_H_ */
