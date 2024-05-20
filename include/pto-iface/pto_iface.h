/*
	PTO Subsystem Interface Header File
	Copyright (c) Norsi-Trans
	Author: o.odintsov@norsi-trans.ru
*/

#ifndef PTO_H_INCLUDED
#define PTO_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILD_PTOIFACE_DLL
#define _PTOAPI __declspec(dllexport)
#else
#ifdef USE_PTOIFACE_DLL
#define _PTOAPI __declspec(dllimport)
#else
#ifdef BUILD_PTOIFACE_SO
#define _PTOAPI __attribute__((visibility ("default")))
#else
#define _PTOAPI
#endif
#endif
#endif


#ifndef DISABLE_PTO_TRANSPORT
#include "../transport/transport.h"
#else
typedef void* transport_point_t;
typedef unsigned transport_pid_t;
typedef long long transport_time_t;
#endif

#include "pto_iface_release.h"

#ifndef DISABLE_PTO_STDARG
#include <stdarg.h>
#endif // DISABLE_PTO_STDARG

struct CFG_NODE; // from cfg_parser

_PTOAPI int  pto_init(transport_point_t htr, int initial_level);
_PTOAPI int  pto_configure(const struct CFG_NODE*config, const char *basepath);
_PTOAPI int  pto_configure_from_file(const char*conf_name, const char*conf_section); // conf_section = NULL for default

_PTOAPI transport_pid_t  pto_get_target_point(); // get lapto point ID or 0 if not configured
_PTOAPI int  pto_set_target_point(transport_pid_t pid); // set lapto point ID or disable if 0


#define PTO_METRICS_NONE	0
#define PTO_METRICS_POINT	1
#define PTO_METRICS_COLLECTD	2
#define PTO_METRICS_GRAPHITE	3

_PTOAPI int  pto_set_meter_target(int meter_type, const char*meter_name);

_PTOAPI void pto_term();



// standard modules
#define PTO_MODULE_COMMON	0 // common messages
#define PTO_MODULE_SYSTEM	1 // messages from operating system
#define PTO_MODULE_TRANSPORT	2 // transport errors
#define PTO_MODULE_PTO		3 // messages from pto interface
#define PTO_MODULE_PRIHD	4 // messages from prihd interface
#define PTO_MODULE_PPK		5 // messages from ppk interface
#define PTO_MODULE_CONFIG	6 // configuration parser
#define PTO_MODULE_TEST		7 // some test module

#define PTO_MODULE_LOCAL_BASE	50 // other local modules
#define PTO_MAX_MODULES		256


#define PTO_MESSAGE_ERROR	0
#define PTO_MESSAGE_WARNING	1
#define PTO_MESSAGE_INFO	2
#define PTO_MESSAGE_DEBUG	3
#define PTO_MESSAGE_TRACE	4
#define PTO_MESSAGE_METER	5
#define PTO_MESSAGE_ACTION	6

#define PTO_MSG_TYPES		16


#define PTO_LEVEL_DEFAULT	0 // level for this msg_id is specified in config
#define PTO_LEVEL_IMPORTANT	1 // important messages
#define PTO_LEVEL_MEDIUM	2 // medium importance
#define PTO_LEVEL_NOTIFY	3 // notify messages
#define PTO_LEVEL_EXTRA		4 // extra messages
#define PTO_LEVEL_DEVEL		5 // developer messages
#define PTO_LEVEL_DEVEL2	6 // second-level developer messages
#define PTO_LEVEL_DEVEL3	7 // third-level developer messages
#define PTO_LEVEL_DEVEL4	8 // fourth-level developer messages
#define PTO_LEVEL_DEVEL5	9 // fifth-level developer messages
#define PTO_LEVEL_ALL		10 // all messages


#define PTO_MAX_LEVELS		16

#define PTO_DEFAULT_GLOBAL_LEVEL	PTO_MAX_LEVELS

#define PTO_MSG_DEFAULT		NULL // message text is selected from config by msg_par


#define PTO_ERROR_GENERIC	(-1)
#define PTO_ERROR_SYNTAX	(-2)
#define PTO_ERROR_CONFIG	(-3)
#define PTO_ERROR_TRANSPORT	(-4)
#define PTO_ERROR_MEMORY	(-5)
#define PTO_ERROR_IO		(-6)

#define pto_global_levels pto_global_levels_2 // because MSG_TYPES changed it's value
#define pto_module_levels pto_module_levels_2 // because MSG_TYPES changed it's value

_PTOAPI extern char pto_global_levels[PTO_MSG_TYPES];
_PTOAPI extern char pto_module_levels[PTO_MSG_TYPES][PTO_MAX_MODULES];


// standard message
#define PTO_MSG_PAR_GENERIC	0 // message text is specified by string (msg)

#ifdef __GNUC__
#ifndef DISABLE_PTO_FORMAT_CHECKS
#define _PTO_PRINTF(N,M) __attribute__ ((format (printf, N, M)))
#else
#define _PTO_PRINTF(N,M)
#endif
#else
#define _PTO_PRINTF(N,M)
#endif

_PTOAPI int pto_msg_f(int module, int msg_t, int msg_lvl, int msg_par, const void*data, int dlen, const char*msg,...) _PTO_PRINTF(7,8);
_PTOAPI int pto_str_msg_f(char *buf, int size, int module, int msg_t, int msg_lvl, int msg_par, const void*data, int dlen, const char*msg,...) _PTO_PRINTF(9,10);
_PTOAPI int pto_meter_int_f(int module, int meter_lvl, transport_time_t t, long long value, const char*fmt,...) _PTO_PRINTF(5,6);
_PTOAPI int pto_meter_real_f(int module, int meter_lvl, transport_time_t t, double value, const char*fmt,...) _PTO_PRINTF(5,6);

#ifndef DISABLE_PTO_STDARG
_PTOAPI int pto_msg_fv(int module, int msg_t, int msg_lvl, int msg_par, const void*data, int dlen, const char*msg, va_list l);
_PTOAPI int pto_str_msg_fv(char*buf, int size, int module, int msg_t, int msg_lvl, int msg_par, const void*data, int dlen, const char*msg, va_list l);
_PTOAPI int pto_meter_int_fv(int module, int meter_lvl, transport_time_t t, long long value, const char*fmt, va_list l);
_PTOAPI int pto_meter_real_fv(int module, int meter_lvl, transport_time_t t, double value, const char*fmt, va_list l);
#endif // DISABLE_PTO_STDARG

#define _pto_active(module, msg_t, msg_lvl, msg_par) (((msg_lvl) <= pto_global_levels[(msg_t)]) && ((msg_lvl) <= pto_module_levels[(msg_t)][(module)]))
#define pto_msg(module, msg_t, msg_lvl, msg_par, ...) do { if (_pto_active(module,msg_t,msg_lvl,msg_par)) pto_msg_f(module, msg_t, msg_lvl, msg_par, NULL, 0, __VA_ARGS__); } while (0)
#define pto_msg_data(module, msg_t, msg_lvl, msg_par, data, dlen, ...) do { if (_pto_active(module,msg_t,msg_lvl,msg_par)) pto_msg_f(module, msg_t, msg_lvl, msg_par, data, dlen, __VA_ARGS__); } while (0)
#define pto_meter_int(module, msg_lvl, t, value, ...) do { if (_pto_active(module,PTO_MESSAGE_METER,msg_lvl,0)) pto_meter_int_f(module, msg_lvl, t, value, __VA_ARGS__); } while (0)
#define pto_meter_real(module, msg_lvl, t, value, ...) do { if (_pto_active(module,PTO_MESSAGE_METER,msg_lvl,0)) pto_meter_real_f(module, msg_lvl, t, value, __VA_ARGS__); } while (0)

#ifndef THIS_MODULE_ID
#define THIS_MODULE_ID PTO_MODULE_COMMON
#endif

/*
Examples of debug macroses for PTO interface
*/

#ifndef DISABLE_PTO_MACROSES

#ifdef PTO_LEVEL_LIMIT
#define __WITH_LEVEL(lvl, cmd) do if ((PTO_LEVEL_##lvl) <= PTO_LEVEL_LIMIT) cmd; while(0)
#else
#define __WITH_LEVEL(lvl, cmd) cmd
#endif

#define ERROR(code,...) pto_msg(THIS_MODULE_ID, PTO_MESSAGE_ERROR, PTO_LEVEL_DEFAULT, code, __VA_ARGS__)
#define STDERROR(code,...) pto_msg(PTO_MODULE_SYSTEM, PTO_MESSAGE_ERROR, PTO_LEVEL_DEFAULT, code, __VA_ARGS__)
#define WARN(lvl,...) __WITH_LEVEL(lvl, pto_msg(THIS_MODULE_ID, PTO_MESSAGE_WARNING, PTO_LEVEL_##lvl, 0,  __VA_ARGS__))
#define INFO(lvl, code, ...) __WITH_LEVEL(lvl, pto_msg(THIS_MODULE_ID, PTO_MESSAGE_INFO, PTO_LEVEL_##lvl, code, __VA_ARGS__))
#define ACTION(lvl, code, ...) __WITH_LEVEL(lvl, pto_msg(THIS_MODULE_ID, PTO_MESSAGE_ACTION, PTO_LEVEL_##lvl, code, __VA_ARGS__))
#define METER_INT(lvl, value, ...) __WITH_LEVEL(lvl, pto_meter_int(THIS_MODULE_ID, PTO_LEVEL_##lvl, TRANSPORT_INVALID_TIME, value, __VA_ARGS__))
#define METER_REAL(lvl, value, ...) __WITH_LEVEL(lvl, pto_meter_real(THIS_MODULE_ID, PTO_LEVEL_##lvl, TRANSPORT_INVALID_TIME, value, __VA_ARGS__))

#define DEBUG_LN(lvl, ln, ...) __WITH_LEVEL(lvl, pto_msg(THIS_MODULE_ID, PTO_MESSAGE_DEBUG, PTO_LEVEL_##lvl, 0, __FILE__ "@" #ln ": " __VA_ARGS__))
#define DEBUG_LN0(lvl, ln, ...) DEBUG_LN(lvl, ln, __VA_ARGS__)
#define DEBUG(lvl, ...) DEBUG_LN0(lvl, __LINE__, __VA_ARGS__)

#define DEBUG_LN_DATA(lvl, ln, p, len, ...) __WITH_LEVEL(lvl, pto_msg_data(THIS_MODULE_ID, PTO_MESSAGE_DEBUG, PTO_LEVEL_##lvl, 0, p, len, __FILE__ "@" #ln ": " __VA_ARGS__))
#define DEBUG_LN0_DATA(lvl, ln, p, len, ...) DEBUG_LN_DATA(lvl, ln, p, len, __VA_ARGS__)
#define DEBUG_DATA(lvl, p, len, ...) DEBUG_LN0_DATA(lvl, __LINE__, p, len, __VA_ARGS__)


#define TRACE(lvl, code, ...) __WITH_LEVEL(lvl, pto_msg(THIS_MODULE_ID, PTO_MESSAGE_TRACE, PTO_LEVEL_##lvl, code, __VA_ARGS__))
#define TRACE_DATA(lvl, code, p, len, ...) __WITH_LEVEL(lvl, pto_msg_data(THIS_MODULE_ID, PTO_MESSAGE_TRACE, PTO_LEVEL_##lvl, code, p, len, __VA_ARGS__))
#define TRACE_PARAMS(int_array) pto_specify_params(THIS_MODULE_ID, int_array, sizeof(int_array) / sizeof(int_array[0]))
#define TRACE_PARAM(val, no) pto_specify_param(THIS_MODULE_ID, val, no)
#define CLEAR_PARAMS() pto_specify_params(THIS_MODULE_ID, NULL, 0)

#define PUSH_PREFIX(...) pto_push_prefix(THIS_MODULE_ID, __VA_ARGS__)
#define POP_PREFIX() pto_pop_prefix(THIS_MODULE_ID)

#ifndef DISABLE_PTO_STDARG
#define PUSH_PREFIXV(...) pto_push_prefixv(THIS_MODULE_ID, __VA_ARGS__)
#endif // DISABLE_PTO_STDARG

#endif // DISABLE_PTO_MACROSES



#define PTO_MAX_PARAMS	16
#define PTO_MAX_PREFIX_LEVELS	16

#ifndef DISABLE_PTO_STDARG
_PTOAPI void pto_push_prefixv(int module, const char*prefix, va_list l);
#endif // DISABLE_PTO_STDARG

_PTOAPI void pto_push_prefix(int module, const char*prefix, ...) _PTO_PRINTF(2,3);
_PTOAPI void pto_pop_prefix(int module);
_PTOAPI void pto_specify_params(int module, const int params[], int n_params);
_PTOAPI void pto_specify_param(int module, int param, int index);
_PTOAPI int  pto_calc_hash(const char*str, int len); // len = -1 -> ASCIIZ

_PTOAPI void pto_update();

_PTOAPI int pto_find_module_id_by_name(const char*name); // returns -1 if not found
_PTOAPI const char* pto_find_module_name_by_id(int id); // returns NULL if not found

_PTOAPI int pto_get_level_id_by_name(const char*name); // returns -1 if not found
_PTOAPI const char* pto_get_level_name_by_id(int id); // returns NULL if not found

_PTOAPI int pto_get_type_id_by_name(const char*name); // returns -1 if not found
_PTOAPI const char* pto_get_type_name_by_id(int id); // returns NULL if not found



#ifdef __cplusplus
} // extern "C"
#endif

#endif // PTO_H_INCLUDED
