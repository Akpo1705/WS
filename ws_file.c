#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ws_file.h"

void getfileurl(char *route, char *fileURL)
{
  char *question = strrchr(route, '?');
  if (question)
    *question = '\0';

  if (route[strlen(route) - 1] == '/')
  {
    strcat(route, "index.html");
  }

  strcpy(fileURL, "htdocs");
  strcat(fileURL, route);

  const char *dot = strrchr(fileURL, '.');
  if (!dot || dot == fileURL)
  {
    strcat(fileURL, ".html");
  }
}

struct file_data *fileload(char *filename){
	char *buffer;

	struct file_data* fd;

	FILE* f = fopen(filename,"r");
	if(f == NULL)
	{
		perror("file open!!!");
		return NULL;
	}

	fseek(f,0, SEEK_END);
	long file_size = ftell(f);
	rewind(f);

	buffer = (char*)malloc((file_size + 1)* sizeof(char));
	if(!buffer){
		return NULL;
	}

	fread(buffer, 1, file_size,f);
	buffer[file_size] = '\0';

	fd = malloc(sizeof (*fd));

	fd->data = buffer;
	fd->size = file_size;

	return fd;
}

void filefree(struct file_data* fd){
	free(fd->data);
	free(fd);
}

void getmimetype(char *file, char *mime)
{
  const char *dot = strrchr(file, '.');

  if (dot == NULL)
    strcpy(mime, "text/html");

  else if (strcmp(dot, ".html") == 0)
    strcpy(mime, "text/html");

  else if (strcmp(dot, ".css") == 0)
    strcpy(mime, "text/css");

  else if (strcmp(dot, ".js") == 0)
    strcpy(mime, "application/js");

  else if (strcmp(dot, ".jpg") == 0)
    strcpy(mime, "image/jpeg");

  else if (strcmp(dot, ".png") == 0)
    strcpy(mime, "image/png");

  else if (strcmp(dot, ".gif") == 0)
    strcpy(mime, "image/gif");

  else
    strcpy(mime, "text/html");
}

char* gethtmlfile(char* filename, char* headers, size_t h_size, int *out_len){

		struct file_data *file = fileload(filename);

		*out_len = h_size+file->size;
		char* out_buf = malloc(h_size+file->size);

		if(out_buf == NULL)
			perror("file.c ----- out buff is bad\n");

		strcpy(out_buf,headers);
		char* p = out_buf + h_size;
		strcpy(p, file->data);

		filefree(file);

		return out_buf;
}

