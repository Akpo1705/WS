#define DEFAULT_SIZE 23
#include "error.h"
#include "node.h"
#include "types.h"

#define ER char *(*)(void *)

typedef struct _Dict {
    Node* hashmap;
    uint64 size, count;
} *Dict;

//? Dict functions
Dict new_dict(uint64 size);

ERROR_CODE dict_add(Dict dict, const char* key, void* value);
ERROR_CODE dict_rem(Dict dict, const char* key, void free_element(Element));
ERROR_CODE resize_dict(Dict dict, const uint64 new_size);
ERROR_CODE free_dict(Dict dict, void free_element(Element));

Element dict_get(Dict dict, const char* key);

char** dict_keys(Dict dict);
void** dict_to_array(Dict dict, ER);
void print_dict(Dict dict, ER, uint64 spacing);
