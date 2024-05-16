#include "include/cfgparser/cfgparser.h"

struct MODULECONF
{
	cfginfo_t config;
};

void module_conf_clear(struct MODULECONF*mc);
void module_conf_free(struct MODULECONF*mc);
int module_conf_load(struct MODULECONF*mc, const char*name);
