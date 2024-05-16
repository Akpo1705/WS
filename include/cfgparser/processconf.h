#ifndef __PROCESSCONF_H
#define __PROCESSCONF_H

/*
	Copyright (c) Norsi-Trans
	Author Oleg A. Odintsov
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "cfgparser.h"
#include "readtable.h"

#define MAX_INCLUDE_LEVEL 10

#define CONF_INCLUDE_REQUIRE_FILES	8 // require all files to be present (return error if no file(s) found)

_CFGAPI cfgres_t conf_process_includes(cfginfo_t cfg, cfgnode_t parent, char basepath_buf[CFGPARSER_MAX_PATH], int level, unsigned flags); // if basepath == NULL, use basepath from config, is parent == NULL, use root




#define CONF_TABLE_BLOCK_NAME_AS_FIRST_COL	1 // treat block name as first column value (if it is not specified)
#define CONF_TABLE_ADD_ROW_ON_DUP_VALUE		2 // automatically add row on duplicating column value
#define CONF_TABLE_IGNORE_UNKNOWNS		4 // ignore unknown parameters (do not return error)
#define CONF_TABLE_REQUIRE_FILES		8 // require all files to be present (return error if no file(s) found)

_CFGAPI cfgres_t conf_process_table(const cfginfo_t cfg, cfgnode_t super, const char**columns, int ncols, table_t td, char basepath_buf[CFGPARSER_MAX_PATH], unsigned flags); // if basepath==NULL, use basepath from config; if suprt == NULL, use root

#ifdef __cplusplus
}
#endif


#endif //__PROCESSCONF_H
