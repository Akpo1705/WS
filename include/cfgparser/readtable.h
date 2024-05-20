/*
	Copyright (c) Norsi-Trans
	Author Oleg A. Odintsov
*/

#ifndef __READTABLE_H
#define __READTABLE_H

#include <stdio.h>

#include "cfgparser_export.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct TABLE_DATA;

typedef struct TABLE_DATA*table_t;

typedef int tableres_t;

#define TABLE_DELIM_AUTO 0

#define TABLE_OK	0
#define TABLE_SYNTAX	(-20)
#define TABLE_IO	(-21)
#define TABLE_MEM	(-22)
#define TABLE_EMPTY	(-23)


#define TABLE_FLAG_STRIP	1 // strip spaces
#define TABLE_FLAG_NO_HEADER	2 // do not parse header, ncols should be specified
#define TABLE_FLAG_QUOTES	4 // allow quoted strings
#define TABLE_FLAG_MANAGE	8 // manage memory
#define TABLE_FLAG_COPY		16// copy memory
#define TABLE_FLAG_EXTERNAL	32// assume memory is managed externally
#define TABLE_FLAG_FIX		64// fix EOLs (modifies memory)

_CFGAPI table_t table_new();
_CFGAPI tableres_t table_read_file(table_t tbl, const char*fname, int delim, unsigned flags);
_CFGAPI tableres_t table_read_mem(table_t tbl, const char*refname, const void*data, int len, int delim, unsigned flags); // data should end with zero, flags should contain manage or copy flag
_CFGAPI void table_free(table_t tbl);

_CFGAPI int table_ncols(const table_t tbl);
_CFGAPI int table_nrows(const table_t tbl);
_CFGAPI const char*table_header(const table_t tbl, int col);
_CFGAPI int table_find_col(const table_t tbl, const char*name); // -1 if not found
_CFGAPI const char*table_value(const table_t tbl, int col, int row);
_CFGAPI const char*table_set_value(const table_t tbl, int col, int row, const char*str, int len, unsigned flags); // flags = TABLE_FLAG_MANAGE, TABLE_FLAG_COPY


_CFGAPI int table_set_cols(table_t tbl, const char**cols, int ncols);
_CFGAPI tableres_t table_append(table_t tbl, const table_t t1, const char**other_cols, const char**other_vals, int nother);
_CFGAPI tableres_t table_append_row(table_t tbl, const char**vals);


_CFGAPI tableres_t table_print(const table_t tbl, FILE*out, int delim, unsigned flags);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //__READTABLE_H
