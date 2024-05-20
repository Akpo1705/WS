#ifdef BUILD_CFGPARSER_DLL
#define _CFGAPI __declspec(dllexport)
#else
#ifdef USE_CFGPARSER_DLL
#define _CFGAPI __declspec(dllimport)
#else
#ifdef BUILD_CFGPARSER_SO
#define _CFGAPI __attribute__((visibility ("default")))
#else
#define _CFGAPI
#endif
#endif
#endif


#ifdef CFGPARSER_EXPORT_PREFIX
#define _CFGEXP2(m,n) m##n
#define _CFGEXP1(m,n) _CFGEXP2(m,n)
#define _CFGEXP(n) _CFGEXP1(CFGPARSER_EXPORT_PREFIX,n)

#define cfgparser_new _CFGEXP(cfgparser_new)
#define cfgparser_load_file _CFGEXP(cfgparser_load_file)
#define cfgparser_load_mem _CFGEXP(cfgparser_load_mem)
#define cfgparser_free _CFGEXP(cfgparser_free)

#define cfgparser_basepath _CFGEXP(cfgparser_basepath)

#define cfgparser_clone _CFGEXP(cfgparser_clone)
#define cfgparser_clone_from _CFGEXP(cfgparser_clone_from)
#define cfgparser_merge _CFGEXP(cfgparser_merge)
#define cfgparser_merge_block _CFGEXP(cfgparser_merge_block)

#define cfgparser_getroot _CFGEXP(cfgparser_getroot)
#define cfgparser_findnode _CFGEXP(cfgparser_findnode)
#define cfgparser_findnode_root _CFGEXP(cfgparser_findnode_root)
#define cfgparser_findpath _CFGEXP(cfgparser_findpath)

#define cfgparser_start _CFGEXP(cfgparser_start)
#define cfgparser_next _CFGEXP(cfgparser_next)

#define cfgparser_is_super _CFGEXP(cfgparser_is_super)
#define cfgparser_is_quoted _CFGEXP(cfgparser_is_quoted)

#define cfgparser_name _CFGEXP(cfgparser_name)
#define cfgparser_value _CFGEXP(cfgparser_value)

#define cfgparser_dump _CFGEXP(cfgparser_dump)
#define cfgparser_dump_nodes _CFGEXP(cfgparser_dump_nodes)

#define cfgparser_get_str _CFGEXP(cfgparser_get_str)
#define cfgparser_get_int _CFGEXP(cfgparser_get_int)
#define cfgparser_get_double _CFGEXP(cfgparser_get_double)
#define cfgparser_get_subnode _CFGEXP(cfgparser_get_subnode)
#define cfgparser_get_enum _CFGEXP(cfgparser_get_enum)
#define cfgstr_get_enum _CFGEXP(cfgstr_get_enum)
#define cfgparser_get_bool _CFGEXP(cfgparser_get_bool)
#define cfgstr_get_bool _CFGEXP(cfgstr_get_bool)

#define cfgparser_parse_params _CFGEXP(cfgparser_parse_params)

#define cfgparser_expand_str _CFGEXP(cfgparser_expand_str)

#define cfgparser_path_expander _CFGEXP(cfgparser_path_expander)
#define cfgparser_section_expander _CFGEXP(cfgparser_section_expander)
#define cfgparser_env_expander _CFGEXP(cfgparser_env_expander)
#define cfgparser_env_path_expander _CFGEXP(cfgparser_env_path_expander)

#define cfgparser_insert_block _CFGEXP(cfgparser_insert_block)

#define conf_process_includes _CFGEXP(conf_process_includes)
#define conf_process_table _CFGEXP(conf_process_table)

#define table_new _CFGEXP(table_new)
#define table_read_file _CFGEXP(table_read_file)
#define table_read_mem _CFGEXP(table_read_mem)
#define table_free _CFGEXP(table_free)

#define table_ncols _CFGEXP(table_ncols)
#define table_nrows _CFGEXP(table_nrows)

#define table_header _CFGEXP(table_header)
#define table_find_col _CFGEXP(table_find_col)

#define table_value _CFGEXP(table_value)
#define table_set_value _CFGEXP(table_set_value)

#define table_set_cols _CFGEXP(table_set_cols)

#define table_append _CFGEXP(table_append)
#define table_append_row _CFGEXP(table_append_row)

#define table_print _CFGEXP(table_print)

#else
#define _CFGEXP(n) n
#endif

