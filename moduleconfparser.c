#define THIS_MODULE_ID  PTO_MODULE_CONFIG
#include "module.h"

void module_conf_clear(struct MODULECONF*mc)
{
	mc->config = cfgparser_new();
}

void module_conf_free(struct MODULECONF*mc)
{
	cfgparser_free(mc->config);
}

int module_conf_load(struct MODULECONF*mc, const char*name)
{
	int r;
	//cfgnode_t node;
	DEBUG(NOTIFY,"loading "MODULE_NAME" configuration from %s....", name);
	r = cfgparser_load_file(mc->config, name, CFGPARSER_FILE_FLAGS_DEFAULT);
	if(r != CFGPARSER_OK)
	{
		ERROR(r, "unable to load "MODULE_NAME" configuration");
		return r;
	}

	r = conf_process_includes(mc->config, NULL, NULL, 0, 0);
	if(r != CFGPARSER_OK)
	{
		ERROR(r, "unable to process includes in %s", name);
		return r;
	}

	DEBUG(EXTRA, MODULE_NAME" configuration OK");
	return CFGPARSER_OK;
}
