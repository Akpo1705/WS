#ifndef _FILELS_H_ // This was just _FILE_H_, but that interfered with Cygwin
#define _FILELS_H_

struct file_data {
    int size;
    void *data;
};

extern struct file_data *fileload(char *filename);
extern void filefree(struct file_data *filedata);
extern void getfileurl(char *route, char *file_URL);
extern void getmimetype(char *file, char *mime);
extern char* gethtmlfile(char* filename, char* headers, size_t headers_size, int *out_len);

#endif
