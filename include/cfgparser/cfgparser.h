/*
	Copyright (c) Norsi-Trans
	Author Oleg A. Odintsov
*/

#ifndef CFGPARSER_H
#define CFGPARSER_H

#include <stdio.h>

#include "cfgparser_release.h"
#include "cfgparser_export.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#define CFGPARSER_MAX_PATH	1024


struct CFG_NODE;
struct CFG_DATA;

typedef struct CFG_DATA* cfginfo_t;
typedef const struct CFG_NODE* cfgnode_t;
typedef int cfgres_t;

#define CFGPARSER_OK	0
#define CFGPARSER_IO	(-1)
#define CFGPARSER_MEM	(-2)
#define CFGPARSER_SYNT	(-3)
#define CFGPARSER_PAR	(-4)


#define CFGPARSER_FILE_FLAGS_DEFAULT	0

#define CFGPARSER_MEM_FLAG_MANAGE	1 // manage memory via realloc or free
#define CFGPARSER_MEM_FLAG_COPY		2 // copy memory

#define CFGPARSER_PARSE_JSON	0x100 // assume JSON format used

_CFGAPI cfginfo_t cfgparser_new();
_CFGAPI cfgres_t cfgparser_load_file(cfginfo_t cfg, const char*fname, unsigned flags);
_CFGAPI cfgres_t cfgparser_load_mem(cfginfo_t cfg, const char*refname, const void*data, int len, unsigned flags); // flags = manage or copy
_CFGAPI void cfgparser_free(cfginfo_t cfg);

_CFGAPI char*cfgparser_basepath(cfginfo_t cfg);

#define CFGPARSER_MERGE_ALL	(-1)
#define CFGPARSER_MERGE_NONE	0
#define CFGPARSER_MERGE_ROOT	1
_CFGAPI cfgres_t cfgparser_clone(cfginfo_t cfg, const cfginfo_t cfg1);
_CFGAPI cfgres_t cfgparser_clone_from(cfginfo_t cfg, const cfgnode_t from);
_CFGAPI cfgres_t cfgparser_merge(cfginfo_t cfg, const cfginfo_t cfg1, int merge_level); // 0 - just append entries, 1 - merge top entries, (-1) merge all levels
_CFGAPI cfgres_t cfgparser_merge_block(cfginfo_t cfg, cfgnode_t parent, const cfginfo_t cfg1, int merge_level); // 0 - just append entries, 1 - merge top entries, (-1) merge all levels

_CFGAPI cfgnode_t cfgparser_getroot(const cfginfo_t cfg);
_CFGAPI cfgnode_t cfgparser_findnode(const cfgnode_t super, const char*name);
_CFGAPI cfgnode_t cfgparser_findnode_root(const cfginfo_t cfg, const char*name);
_CFGAPI cfgnode_t cfgparser_findpath(cfgnode_t super, const char*path, const char*delims);


typedef int cfgenum_t;

_CFGAPI cfgenum_t cfgparser_start(cfgnode_t super, cfgnode_t*p);
_CFGAPI void cfgparser_next(cfgnode_t*p, cfgenum_t*rm);


#ifdef __cplusplus
#define CFGPARSER_FOREACH(super,p) for(cfgenum_t __rm = cfgparser_start(super, &p); __rm > 0; cfgparser_next(&p, &__rm))
#else
#define CFGPARSER_FOREACH(super,p) cfgenum_t __rm; for(__rm = cfgparser_start(super, &p); __rm > 0; cfgparser_next(&p, &__rm))
#endif

#define CFGPARSER_SUPER_BLOCK	1
#define CFGPARSER_SUPER_ARRAY	2 // to support json arrays

_CFGAPI int cfgparser_is_super(cfgnode_t nd);  // is value block (object,array)?
_CFGAPI int cfgparser_is_quoted(cfgnode_t nd); // is value quoted?
_CFGAPI const char* cfgparser_name(cfgnode_t nd);
_CFGAPI const char* cfgparser_value(cfgnode_t nd);


#define CFGPARSER_DUMP_DEFAULT	0 // dump in original format
#define CFGPARSER_DUMP_LINEAR	1 // dump in linear format
#define CFGPARSER_DUMP_INI	2 // dump in windows ini-file format
#define CFGPARSER_DUMP_JSON	3 // dump in json format
#define CFGPARSER_DUMP_XML	4 // dump in xml format
#define CFGPARSER_DUMP_MASK	0xFF // format mask
#define CFGPARSER_DUMP_NOROOT	0x100 // skip root section, dump contents only
#define CFGPARSER_DUMP_COMPACT	0x200 // compact format for dumping

_CFGAPI int cfgparser_dump(const cfginfo_t cfg, FILE*out, unsigned flags);
_CFGAPI int cfgparser_dump_nodes(const cfgnode_t node, FILE*out, unsigned flags);


_CFGAPI const char*cfgparser_get_str(cfgnode_t super, const char*name, const char*def_val);
_CFGAPI int cfgparser_get_int(cfgnode_t super, const char*name, int def_val);
_CFGAPI double cfgparser_get_double(cfgnode_t super, const char*name, double def_val);
_CFGAPI cfgnode_t cfgparser_get_subnode(cfgnode_t super, const char*name);
_CFGAPI int cfgparser_get_enum(cfgnode_t super, const char*name, const char*names[], int n_names, int start, int def_val); // n_names == -1 -> stop on NULL name
_CFGAPI int cfgstr_get_enum(const char*str, const char*names[], int n_names, int start, int def_val);  // n_names == -1 -> stop on NULL name
_CFGAPI int cfgparser_get_bool(cfgnode_t super, const char*name, int def_val);
_CFGAPI int cfgstr_get_bool(const char*str, int def_val);

_CFGAPI int cfgparser_parse_params(const char*cfg_params, int(*parser)(const char*par, const char*val, void*ptr), void*ptr); // general format: a=b,c,d=e

_CFGAPI int cfgparser_expand_str(const char*src, int srclen, char*dest, int maxlen, int(*expander)(const char*par, int par_len, char*dest, int dest_len, void*ptr), void*ptr); // expand string of format $XXX or ${XXX}
_CFGAPI int cfgparser_path_expander(const char*par, int par_len, char*dest, int dest_len, void*super);
_CFGAPI int cfgparser_section_expander(const char*par, int par_len, char*dest, int dest_len, void*super);
_CFGAPI int cfgparser_env_expander(const char*par, int par_len, char*dest, int dest_len, void*unused);
_CFGAPI int cfgparser_env_path_expander(const char*par, int par_len, char*dest, int dest_len, void*super);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //CFGPARSER_H
