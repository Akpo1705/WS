/*
	Transport Subsystem Interface Header File
	Copyright (c) Norsi-Trans
	Author: o.odintsov@norsi-trans.ru
*/

#ifndef TRANSPORT_H
#define TRANSPORT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "transport_limits.h"
#include "transport_errors.h"


#ifdef BUILD_TRANSPORT_DLL
#define _TRAPI __declspec(dllexport)
#else
#ifdef USE_TRANSPORT_DLL
#define _TRAPI __declspec(dllimport)
#else
#ifdef BUILD_TRANSPORT_SO
#define _TRAPI __attribute__((visibility ("default")))
#else
#define _TRAPI
#endif
#endif
#endif

#include <time.h>

#include "transport_version.h"
#include "transport_release.h"

typedef unsigned int transport_id_t; // generic transport identifier

typedef transport_id_t transport_pid_t; // point identifier
typedef transport_id_t transport_mid_t; // message identifier (number)
typedef transport_id_t transport_eid_t; // event identifier
typedef transport_id_t transport_did_t; // data identifier

typedef void* transport_handle_t; // generic transport handle
typedef transport_handle_t transport_point_t; // handle for connection point
typedef transport_handle_t transport_packet_t; // handle for packet

typedef int transport_res_t; // result
typedef unsigned transport_flags_t; // flags


typedef unsigned long long transport_time_t; // timestamp of events, high 34 bits = seconds like in time_t, low 30 bits = fractions of seconds

#define TRANSPORT_TIME_SHIFT_BITS	30
#define TRANSPORT_TIME_ONE_SECOND	(1ULL << TRANSPORT_TIME_SHIFT_BITS)

typedef unsigned long long transport_large_offset_t;
typedef unsigned long long transport_large_length_t;

#include "transport_config.h"


#define TRANSPORT_POINT_ORDINARY	1 // point-point communication
#define TRANSPORT_POINT_MANY		2 // one-to-many communication
#define TRANSPORT_POINT_CLUSTER		3 // one-to-some communication

// generic protocol levels
#define TRANSPORT_PROTO_GENERIC_MESSAGES	1 // simple messages
#define TRANSPORT_PROTO_GENERIC_REQUESTS	2 // request-response
#define TRANSPORT_PROTO_GENERIC_STREAM		3 // streaming data
#define TRANSPORT_PROTO_GENERIC_LARGE		4 // large data

#define TRANSPORT_PROTO_GENERIC_MASK		15 // 4 bits

#define TRANSPORT_PROTO_DELIVERY_FLAG	16 // guaranteed delivery
#define TRANSPORT_PROTO_INITIATOR_FLAG	32 // initiator side of connection
#define TRANSPORT_PROTO_CONNECT_ON_DEMAND_FLAG 64 // connect-on-demand flag, for use with INITIATOR_FLAG only

#define TRANSPORT_PROTO_EVENTS		(TRANSPORT_PROTO_GENERIC_MESSAGES | TRANSPORT_PROTO_DELIVERY_FLAG) // simple events with guaranteed delivery
#define TRANSPORT_PROTO_NOTIFICATIONS	TRANSPORT_PROTO_GENERIC_MESSAGES // simple notifications
#define TRANSPORT_PROTO_REQUESTS	(TRANSPORT_PROTO_GENERIC_REQUESTS | TRANSPORT_PROTO_DELIVERY_FLAG) // requests with guaranteed delivery
#define TRANSPORT_PROTO_RT_STREAM	TRANSPORT_PROTO_GENERIC_STREAM // real-time stream
#define TRANSPORT_PROTO_DB_STREAM	(TRANSPORT_PROTO_GENERIC_STREAM | TRANSPORT_PROTO_DELIVERY_FLAG) // streaming data from database
#define TRANSPORT_PROTO_LARGE		(TRANSPORT_PROTO_GENERIC_LARGE | TRANSPORT_PROTO_DELIVERY_FLAG) // large data witg guaranteed delivery


#define TRANSPORT_HOST_TYPE_REMOTE	1 // remote host, IP transport
#define TRANSPORT_HOST_TYPE_LOCAL	2 // local host, shared memory transport


#define TRANSPORT_DATA_TYPE_OFS		0

#define TRANSPORT_DATA_TYPE_NULL	0 // null (void) type, may be used as a marker
#define TRANSPORT_DATA_TYPE_INTEGER	1
#define TRANSPORT_DATA_TYPE_REAL	2
#define TRANSPORT_DATA_TYPE_STRING	3
#define TRANSPORT_DATA_TYPE_ARRAY	4 // may be or-ed with other types
#define TRANSPORT_DATA_TYPE_STRUCT	8

#define TRANSPORT_DATA_TYPE_ATOMIC_MASK	3
#define TRANSPORT_DATA_TYPE_MASK	15


#define TRANSPORT_DATA_SIZE_OFS		4

#define TRANSPORT_DATA_SIZE_NULL	0
#define TRANSPORT_DATA_SIZE_BYTE	(1<<TRANSPORT_DATA_SIZE_OFS)
#define TRANSPORT_DATA_SIZE_WORD	(2<<TRANSPORT_DATA_SIZE_OFS)
#define TRANSPORT_DATA_SIZE_DWORD	(4<<TRANSPORT_DATA_SIZE_OFS)
#define TRANSPORT_DATA_SIZE_QWORD	(8<<TRANSPORT_DATA_SIZE_OFS)

#define TRANSPORT_DATA_SIZE_SINGLE	(4<<TRANSPORT_DATA_SIZE_OFS)
#define TRANSPORT_DATA_SIZE_DOUBLE	(8<<TRANSPORT_DATA_SIZE_OFS)

#define TRANSPORT_DATA_SIZE_MASK	(15<<TRANSPORT_DATA_SIZE_OFS)

// valid combinations of data bits
#define TRANSPORT_DATA_NULL	(TRANSPORT_DATA_TYPE_NULL | TRANSPORT_DATA_SIZE_NULL)
#define TRANSPORT_DATA_BYTE	(TRANSPORT_DATA_TYPE_INTEGER | TRANSPORT_DATA_SIZE_BYTE)
#define TRANSPORT_DATA_WORD	(TRANSPORT_DATA_TYPE_INTEGER | TRANSPORT_DATA_SIZE_WORD)
#define TRANSPORT_DATA_DWORD	(TRANSPORT_DATA_TYPE_INTEGER | TRANSPORT_DATA_SIZE_DWORD)
#define TRANSPORT_DATA_QWORD	(TRANSPORT_DATA_TYPE_INTEGER | TRANSPORT_DATA_SIZE_QWORD)
#define TRANSPORT_DATA_SINGLE	(TRANSPORT_DATA_TYPE_REAL | TRANSPORT_DATA_SIZE_SINGLE)
#define TRANSPORT_DATA_DOUBLE	(TRANSPORT_DATA_TYPE_REAL | TRANSPORT_DATA_SIZE_DOUBLE)

#define TRANSPORT_DATA_STRING	(TRANSPORT_DATA_TYPE_STRING | TRANSPORT_DATA_SIZE_BYTE)
#define TRANSPORT_DATA_STRUCT	(TRANSPORT_DATA_TYPE_STRUCT | TRANSPORT_DATA_SIZE_BYTE)

#define TRANSPORT_ARRAY_BYTE	(TRANSPORT_DATA_BYTE | TRANSPORT_DATA_TYPE_ARRAY)
#define TRANSPORT_ARRAY_WORD	(TRANSPORT_DATA_WORD | TRANSPORT_DATA_TYPE_ARRAY)
#define TRANSPORT_ARRAY_DWORD	(TRANSPORT_DATA_DWORD | TRANSPORT_DATA_TYPE_ARRAY)
#define TRANSPORT_ARRAY_QWORD	(TRANSPORT_DATA_QWORD | TRANSPORT_DATA_TYPE_ARRAY)
#define TRANSPORT_ARRAY_SINGLE	(TRANSPORT_DATA_SINGLE | TRANSPORT_DATA_TYPE_ARRAY)
#define TRANSPORT_ARRAY_DOUBLE	(TRANSPORT_DATA_DOUBLE | TRANSPORT_DATA_TYPE_ARRAY)


#define TRANSPORT_CODE_BITS	16 // bits for specific code

#define TRANSPORT_SUBSYSTEM_OFFSET	25
#define TRANSPORT_SUBSYSTEM_BITS	6

#define TRANSPORT_TYPE_OFFSET	TRANSPORT_CODE_BITS
#define TRANSPORT_TYPE_BITS	9

#define TRANSPORT_PROTOCOL_OFFSET	TRANSPORT_CODE_BITS
#define TRANSPORT_PROTOCOL_BITS		9

#define DECLARE_POINT_ID(subsystem, protocol, module) (((subsystem) << TRANSPORT_SUBSYSTEM_OFFSET) | ((protocol) << TRANSPORT_PROTOCOL_OFFSET) | module) // protocol - highest supported protocol level
#define DECLARE_EVENT_ID(subsystem, protocol, code)   (((subsystem) << TRANSPORT_SUBSYSTEM_OFFSET) | ((protocol) << TRANSPORT_PROTOCOL_OFFSET) | code)
#define DECLARE_DATA_ID(subsystem, type, code) (((subsystem) << TRANSPORT_SUBSYSTEM_OFFSET) | ((type) << TRANSPORT_TYPE_OFFSET) | code)

#define POINT_ID_TO_SUBSYSTEM(id) (((id) >> TRANSPORT_SUBSYSTEM_OFFSET) & ((1<<TRANSPORT_SUBSYSTEM_BITS) - 1))
#define POINT_ID_TO_PROTOCOL(id) (((id) >> TRANSPORT_PROTOCOL_OFFSET) & ((1<<TRANSPORT_PROTOCOL_BITS) - 1))
#define POINT_ID_TO_MODULE(id) ((id) & ((1<<TRANSPORT_PROTOCOL_OFFSET) - 1))

#define EVENT_ID_TO_SUBSYSTEM(id) (((id) >> TRANSPORT_SUBSYSTEM_OFFSET) & ((1<<TRANSPORT_SUBSYSTEM_BITS) - 1))
#define EVENT_ID_TO_PROTOCOL(id) (((id) >> TRANSPORT_PROTOCOL_OFFSET) & ((1<<TRANSPORT_PROTOCOL_BITS) - 1))
#define EVENT_ID_TO_CODE(id) ((id) & ((1<<TRANSPORT_PROTOCOL_OFFSET) - 1))

#define TRANSPORT_EVENT_RESPONSE (1L<<31)
#define TRANSPORT_EVENT_MASK (~TRANSPORT_EVENT_RESPONSE)
#define EVENT_IS_RESPONSE(id) (id & TRANSPORT_EVENT_RESPONSE) // to distinguish between requests and responses

#define DATA_ID_TO_SUBSYSTEM(id) (((id) >> TRANSPORT_SUBSYSTEM_OFFSET) & ((1<<TRANSPORT_SUBSYSTEM_BITS) - 1))
#define DATA_ID_TO_TYPE(id) (((id) >> TRANSPORT_TYPE_OFFSET) & ((1<<TRANSPORT_TYPE_BITS) - 1))
#define DATA_ID_TO_CODE(id) ((id) & ((1<<TRANSPORT_TYPE_OFFSET) - 1))

#define TRANSPORT_INVALID_POINT_ID DECLARE_POINT_ID(0,0,0)
#define TRANSPORT_INVALID_EVENT_ID DECLARE_EVENT_ID(0,0,0)
#define TRANSPORT_INVALID_DATA_ID  DECLARE_DATA_ID(0,0,0)
#define TRANSPORT_INVALID_TIME		0
#define TRANSPORT_INVALID_MESSAGE_ID	0
#define TRANSPORT_INVALID_HANDLE	((transport_handle_t)0)

#include "transport_subsystems.h"
#include "transport_events.h"
#include "transport_datacodes.h"

struct TRANSPORT_CONFIG;
struct TRANSPORT_POINT_INFO;


#define TRANSPORT_CONFIG_VER1	0 // original configuration file format
#define TRANSPORT_CONFIG_VER2	1 // new configuration file format

#define TRANSPORT_CONFIG_DEFAULT (TRANSPORT_CONFIG_VER2) // default configuration flags


// Compatibility definitions

#ifdef _FROM_LIB
#define transport_initialize_from_config_compat transport_initialize_from_config
#define transport_reconfigure_from_config_compat transport_reconfigure_from_config
#else
#ifdef _TRANSPORT_OLD_SYNTAX
#define transport_initialize_from_config_compat transport_initialize_from_config
#define transport_reconfigure_from_config_compat transport_reconfigure_from_config
#else
#define transport_initialize_from_config transport_initialize_from_config_ver2
#define transport_reconfigure_from_config transport_reconfigure_from_config_ver2
#endif
#endif



// initialize transport with specified configuration, establish all required connections if necessary
_TRAPI transport_point_t  transport_initialize(struct TRANSPORT_CONFIG*cfg, transport_pid_t id);
// initialize transport without point
_TRAPI transport_point_t  transport_initialize_no_point();
// helper function, initialize from config
_TRAPI transport_point_t  transport_initialize_from_config_ver2(const char*fname, const char*point_name, unsigned flags);
// compatibility function
_TRAPI transport_point_t  transport_initialize_from_config_compat(const char*fname, const char*point_name);
// disconnect and terminate all transfers, free all in-core data
_TRAPI void transport_terminate(transport_point_t h);

_TRAPI transport_res_t transport_is_point_started(transport_point_t h); // returns TRANSPORT_OK if started or TRANSPORT_ERROR if not
_TRAPI transport_res_t transport_is_peer_working(transport_point_t h, transport_pid_t pid); // returns TRANSPORT_OK if working or TRANSPORT_ERROR if not
_TRAPI transport_res_t transport_format_peer_info(transport_point_t h, transport_pid_t pid, const char*format, char*buffer, int buflen); // returns TRANSPORT_OK if ok, TRANSPORT_NO_MEMORY if buffer small, TRANSPORT_INV_PARAM if h is invalid, TRANSPORT_NO_DATA if no peer found
// format: 
//	%n% - point name,
//	%s% - subsystem id,
//	%l% - protocol level,
//	%m% - module id,
//	%t% - point type
//	%x% - ID in hex format
//
//	%h% - host name,
//	%T% - host type (LOCAL/REMOTE),
//	%a% - outgoing (server) address for remote hosts,
//	%p% - outgoing (server) port number,
//	%A% - incoming (client) address for remote points,
//	%P% - incoming (client) port number
//	%S% - connection state

_TRAPI transport_res_t transport_reconfigure(transport_point_t h, struct TRANSPORT_CONFIG*cfg);
_TRAPI transport_res_t transport_reconfigure_from_config_ver2(transport_point_t h, const char*fname, unsigned flags); // doesn't require termination of point, fname may be NULL to reread current config, returns TRANSPORT_OK if configuration has been successfully updated or other code if some error occured
// compatibility function
_TRAPI transport_res_t transport_reconfigure_from_config_compat(transport_point_t h, const char*fname);

_TRAPI transport_res_t transport_register_handler(const void*adr, const char*name); // to preserve handlers during caching, need to be called before any transport initialization function executed
#define TRANSPORT_REGISTER_HANDLER(func) transport_register_handler((const void*)func, #func) // helper macros


#define TRANSPORT_CLEAR_UNSENT	1	// clear packets ready to send
#define TRANSPORT_CLEAR_INCOMPLETE	2	// clear active requests
#define TRANSPORT_CLEAR_STREAM	4	// clear active streams
#define TRANSPORT_CLEAR_LARGE	8	// clear active large transfers
#define TRANSPORT_CLEAR_ALL	0x0F

_TRAPI int transport_get_peer_pending_packets_count(transport_point_t h, transport_pid_t pid, unsigned clear_set); // request pending packets count, do not clear them
_TRAPI int transport_get_pending_packets_count(transport_point_t h, unsigned clear_set); // request pending packets count, do not clear them

_TRAPI transport_res_t transport_clear_peer_packets(transport_point_t h, transport_pid_t pid, unsigned clear_set); // clear packets for specified peer
_TRAPI transport_res_t transport_clear_packets(transport_point_t h, unsigned clear_set); // clear packets for all peers


#define TRANSPORT_START_FAIL_ON_ERROR	0x01	// fail on any connection error to peers
#define TRANSPORT_START_DELAY_CONNECT	0x02	// delay connection to permanent peers
#define TRANSPORT_START_CONNECT_ALL	0x04	// connect to all nodes, including cluster (default is to connect only to a single-point peers)
#define TRANSPORT_DISABLE_CACHE		0x08	// disable cacheing for this point
#define TRANSPORT_DISABLE_PING		0x10	// do not automatically answer ping events
#define TRANSPORT_ENABLE_FAST		0x20	// enable "fast" transmits
#define TRANSPORT_DISABLE_BUFFER	0x40	// disable TCP buffering
#define TRANSPORT_DISABLE_RECONNECT	0x80	// do not reconnect permanent peers
#define TRANSPORT_DISABLE_PROC		0x100	// disable internal handling of process requests
#define TRANSPORT_DISABLE_KEEPALIVE	0x200	// disable keep-alive tcp packets

_TRAPI transport_res_t transport_start_point(transport_point_t h, unsigned flags); // start point after initialization
_TRAPI transport_res_t transport_stop_point(transport_point_t h); // stop point to reconfigure/terminate

// callback functions declarations
typedef void(*transport_debug_callback_t)(void*ptr, int lvl, const char*msg);
typedef transport_res_t (*transport_request_callback_t)(void*ptr, transport_packet_t request);
typedef transport_res_t (*transport_response_callback_t)(void*ptr, transport_packet_t request, transport_packet_t response);
typedef void (*transport_free_callback_t)(void*ptr, transport_packet_t packet);
typedef transport_res_t (*transport_error_callback_t)(void*ptr, transport_packet_t packet, transport_res_t cause); // TRANSPORT_FINISH to stop current operation


#define TRANSPORT_DEBUG_CODE_SERVICE_START	100	// special service codes
#define TRANSPORT_DEBUG_CODE_START	101	// code to start point
#define TRANSPORT_DEBUG_CODE_STOP	102	// code to stop point
#define TRANSPORT_DEBUG_CODE_UPDATE	103	// code to update point
#define TRANSPORT_DEBUG_CODE_FREE	110	// code to free point
// CALLBACK on debug message
_TRAPI transport_res_t transport_set_debug_callback(transport_point_t h, transport_debug_callback_t callback, void*cb_ptr); // h = NULL to set default callback
_TRAPI const struct TRANSPORT_CONFIG* transport_get_point_config(transport_point_t h); // request current point configuration

_TRAPI const char* transport_get_error_str(int code);
#define TRANSPORT_FAILED(code) ((code) < TRANSPORT_OK)


// MESSAGE PACKETS
_TRAPI transport_packet_t transport_create_packet(transport_point_t h, transport_pid_t target, transport_eid_t event); // allocate packet
#define transport_create_temp_packet(h) transport_create_packet(h, TRANSPORT_INVALID_POINT_ID, TRANSPORT_INVALID_EVENT_ID) // create temporary packet associated with specified point
#define transport_create_mem_packet() transport_create_packet(TRANSPORT_INVALID_HANDLE, TRANSPORT_INVALID_POINT_ID, TRANSPORT_INVALID_EVENT_ID) // create temporary packet in memory

_TRAPI transport_packet_t transport_create_response(transport_packet_t packet); // create response packet
_TRAPI transport_packet_t transport_duplicate_packet(transport_packet_t packet); // duplicate specified packet
_TRAPI transport_res_t transport_set_packet_data(transport_packet_t packet, void*data); // set local user data
_TRAPI transport_res_t transport_get_packet_data(transport_packet_t packet, void**data); // get local user data
_TRAPI transport_res_t transport_reuse_packet(transport_packet_t packet, transport_point_t h, transport_pid_t target, transport_eid_t event); // reuse already allocated packet with all data
_TRAPI transport_res_t transport_store_packet(transport_packet_t packet); // store received packet in memory, disable auto-deallocation
_TRAPI transport_res_t transport_append_packet(transport_packet_t this_packet, transport_packet_t packet_to_append); // append data of packet
_TRAPI transport_res_t transport_push_packet(transport_packet_t packet); // send repeating packets (use transport_get_array_ptr)
_TRAPI transport_res_t transport_send_packet(transport_packet_t packet); // send packet and free resources
_TRAPI transport_res_t transport_push_packet_to(transport_packet_t packet, transport_pid_t target); // send repeating packets to different targets
_TRAPI void transport_clear_packet(transport_packet_t packet); // remove any packet data
_TRAPI void transport_free_packet(transport_packet_t packet); // just free packet resources

_TRAPI transport_res_t transport_is_packet_final(transport_packet_t packet); // returns FINISH if packet is marked as final or OK if not

#define TRANSPORT_PACKET_SIZE_USED	0 // memory used by packet contents
#define TRANSPORT_PACKET_SIZE_ALLOCATED	1 // memory allocated from heap
#define TRANSPORT_PACKET_SIZE_FREE	2 // = ALLOCATED - USED

_TRAPI int transport_get_packet_size(transport_packet_t packet, int which); // get information about packet memory allocation
_TRAPI transport_res_t transport_enlarge_packet_size(transport_packet_t packet, int delta); // preallocate packet buffer

// internal use only
_TRAPI transport_res_t _transport_get_packet_contents(transport_packet_t packet, void**data, int*len);
_TRAPI transport_res_t _transport_set_packet_contents(transport_packet_t packet, const void*data, int len);


#define TRANSPORT_ERROR_DEFAULT		0x00 // default action
#define TRANSPORT_ERROR_RETRY		0x10 // resend packet on send error
#define TRANSPORT_ERROR_CALLBACK	0x20 // exec callback on error
#define TRANSPORT_ERROR_DELETE		0x30 // delete packet on error

_TRAPI transport_res_t transport_set_packet_error_action(transport_packet_t packet, int timeout, int action, transport_error_callback_t callback, void*cb_ptr);

#define TRANSPORT_TIMEOUT_DEFAULT	0 // default action
#define TRANSPORT_TIMEOUT_RETRY		1 // resend request on timeout
#define TRANSPORT_TIMEOUT_CALLBACK	2 // exec specified callback function on timeout
#define TRANSPORT_TIMEOUT_DELETE	3 // delete request on timeout

_TRAPI transport_res_t transport_set_request_timeout_action(transport_packet_t packet, int timeout, int action, transport_error_callback_t callback, void*cb_ptr);

struct TRANSPORT_ARRAY_INFO
{
	void*data;
	int length; // in elements
	int el_size; // element size in bytes
};

// DATA for PACKET
_TRAPI transport_res_t transport_add_null(transport_packet_t packet, transport_did_t data_id); // add data tag without value (DATA_NULL)
_TRAPI transport_res_t transport_add_integer(transport_packet_t packet, transport_did_t data_id, int value);
_TRAPI transport_res_t transport_add_long(transport_packet_t packet, transport_did_t data_id, long long value);
_TRAPI transport_res_t transport_add_real(transport_packet_t packet, transport_did_t data_id, double value);
_TRAPI transport_res_t transport_add_string(transport_packet_t packet, transport_did_t data_id, const char*str, int nchars); // nchars = -1 for ASCIIZ strings
_TRAPI transport_res_t transport_add_array(transport_packet_t packet, transport_did_t data_id, int n_elements, const void*data); // data may be NULL to just allocate space with some random data
_TRAPI transport_res_t transport_add_array_ex(transport_packet_t packet, transport_did_t data_id, int n_elements, struct TRANSPORT_ARRAY_INFO*ainf); // allocates data and returns pointer to it in ainf

_TRAPI transport_res_t transport_append_string(transport_packet_t packet, transport_did_t data_id, const char*str, int nchars); // nchars = -1 for ASCIIZ strings
_TRAPI transport_res_t transport_append_array(transport_packet_t packet, transport_did_t data_id, int n_elements, const void*data); // data may be NULL to just allocate space with some random data
_TRAPI transport_res_t transport_append_array_ex(transport_packet_t packet, transport_did_t data_id, int n_elements, struct TRANSPORT_ARRAY_INFO*ainf); // append array and return pointer to start of new elements

_TRAPI transport_res_t transport_incapsulate_packet(transport_packet_t packet, transport_did_t data_id, transport_packet_t p1); // data_id must define struct, p1 - packet to be incapsulated
_TRAPI transport_res_t transport_add_data_from_struct(transport_packet_t packet, transport_did_t data_id, transport_packet_t p1); // data_id must define struct, p1 - packet containing struct

_TRAPI transport_res_t transport_begin_struct(transport_packet_t packet, transport_did_t data_id); // data_id must define struct
_TRAPI transport_res_t transport_end_struct(transport_packet_t packet); // finish last struct
_TRAPI transport_res_t transport_end_struct_not_empty(transport_packet_t packet); // finish last struct if it has data, remove empty struct, return TRANSPORT_FINISH on remove

_TRAPI transport_res_t transport_enter_struct(transport_packet_t packet, transport_did_t data_id); // data_id must define struct
_TRAPI transport_res_t transport_leave_struct(transport_packet_t packet); // leave last struct

_TRAPI transport_res_t transport_replace_integer(transport_packet_t packet, transport_did_t data_id, int value);
_TRAPI transport_res_t transport_replace_long(transport_packet_t packet, transport_did_t data_id, long long value);
_TRAPI transport_res_t transport_replace_real(transport_packet_t packet, transport_did_t data_id, double value);
_TRAPI transport_res_t transport_replace_string(transport_packet_t packet, transport_did_t data_id, const char*str, int nchars); // nchars = -1 for ASCIIZ strings
_TRAPI transport_res_t transport_replace_array(transport_packet_t packet, transport_did_t data_id, int n_elements, const void*data); // data may be NULL to just allocate space with some random data
_TRAPI transport_res_t transport_replace_array_ex(transport_packet_t packet, transport_did_t data_id, int n_elements, struct TRANSPORT_ARRAY_INFO*ainf); // resize array and return pointer to a new area

_TRAPI transport_res_t transport_remove_data(transport_packet_t packet, transport_did_t data_id); // remove data of any type from packet (to reuse the packet)
_TRAPI transport_res_t transport_copy_data(transport_packet_t source, transport_packet_t dest, transport_did_t data_id); // copy specified field from one packet to another
_TRAPI transport_res_t transport_copy_data_ex(transport_packet_t source, transport_did_t src_data_id, transport_packet_t dest, transport_did_t dest_data_id); // copy specified field from one packet to another with id changes
_TRAPI transport_res_t transport_is_data_present(transport_packet_t packet, transport_did_t data_id); // returns OK or DATA_NOT_FOUND

_TRAPI int transport_get_integer(transport_packet_t packet, transport_did_t data_id, int def_val);
_TRAPI long long transport_get_long(transport_packet_t packet, transport_did_t data_id, long long def_val);
_TRAPI double transport_get_real(transport_packet_t packet, transport_did_t data_id, double def_val);
_TRAPI const char*transport_get_string(transport_packet_t packet, transport_did_t data_id, const char*def_val); // def_val may be NULL
_TRAPI const void*transport_get_array_ptr(transport_packet_t packet, transport_did_t data_id, int start_offset); // returns NULL if data_id or start_offset are invalid; may be used to send streaming data via transport_push_packet
_TRAPI int transport_get_array_length(transport_packet_t packet, transport_did_t data_id); // returns (-1) if no array found

typedef transport_res_t (*transport_data_callback_t)(void*ptr, transport_did_t data_id, const void*data_ptr, int elem_size, int elem_count); // returns TRANSPORT_FINISH to stop enumeration
_TRAPI transport_res_t transport_enumerate_packet_data(transport_packet_t packet, transport_data_callback_t callback, void*cb_ptr); // enumerate packet contents
_TRAPI transport_res_t transport_enumerate_struct_data(transport_packet_t packet, const void*struct_ptr, int struct_len, transport_data_callback_t callback, void*cb_ptr); // enumerate structure contents

#define TRANSPORT_PACKET_ENUM_START		0 // always search from start
#define TRANSPORT_PACKET_ENUM_CONTINUE		1 // continue to search untill end

_TRAPI transport_res_t transport_set_packet_enum_mode(transport_packet_t packet, int mode); // set packet enumerate mode
_TRAPI transport_res_t transport_reset_packet_enum_offset(transport_packet_t packet); // reset offset to zero for continue enum modes

// alternative more fast function
_TRAPI transport_res_t transport_get_array_info(transport_packet_t packet, transport_did_t data_id, struct TRANSPORT_ARRAY_INFO*info); // returns OK or DATA_NOT_FOUND



// flags in export/import
#define TRANSPORT_DATA_FORMAT_TEXT		1 // export in text format
#define TRANSPORT_DATA_FORMAT_XML		2 // export in xml format
#define TRANSPORT_DATA_FORMAT_HTML		3 // export in html format
#define TRANSPORT_DATA_FORMAT_RAW		4 // export in raw format

#define TRANSPORT_DATA_FORMAT_MASK		0x00F // maximum number of possible export formats

#define TRANSPORT_DATA_FIELD_ID			16 // export data ID code in text format
#define TRANSPORT_DATA_FIELD_TYPE		32 // export data type code in text format
#define TRANSPORT_DATA_FIELD_VALUE		64 // export data value code in text format
#define TRANSPORT_DATA_FIELD_MASK		0x0F0 // bits for data fields

#define TRANSPORT_DATA_FIELD_ALL		TRANSPORT_DATA_FIELD_MASK

#define TRANSPORT_EXPORT_TO_TEXT		(TRANSPORT_DATA_FORMAT_TEXT | TRANSPORT_DATA_FIELD_ALL)
#define TRANSPORT_IMPORT_FROM_TEXT		(TRANSPORT_DATA_FORMAT_TEXT | TRANSPORT_DATA_FIELD_ALL)


#define TRANSPORT_EXPORT_STATIC			0x000	// assume that buf is a static buffer, no realloc, return error if it is too small
#define TRANSPORT_EXPORT_ALLOCATE		0x100	// allocate buffer automatically, initial pointer is not valid, free(...) should be called to free the buffer
#define TRANSPORT_EXPORT_REALLOC		0x200	// assume that buf is already allocated by malloc (or NULL), reallocate when necessary, free(...) should be called to free the buffer
#define TRANSPORT_EXPORT_INTERNAL		0x300	// only for raw format, return pointer to data block (no need to call free(...))
#define TRANSPORT_EXPORT_MEMORY_MASK		0xF00

#define TRANSPORT_IMPORT_NO_DUPS	0x1000	// ignore duplicate values (including already present in the packet)

#define TRANSPORT_EXPORT_DATA_ALL	TRANSPORT_INVALID_DATA_ID // for data_id, export all data

_TRAPI transport_res_t transport_export_data(transport_packet_t packet, transport_did_t data_id, void**buf, int*buflen, unsigned flags);
_TRAPI transport_res_t transport_import_data(transport_packet_t packet, const void*buf, int buflen, unsigned flags);


// CALLBACK on packet REQUEST/NOTIFICATION
_TRAPI transport_res_t transport_set_request_callback(transport_point_t h, transport_request_callback_t callback, void*cb_ptr);
// CALLBACK on packet RESPONSE
_TRAPI transport_res_t transport_set_response_callback(transport_packet_t pkt, transport_response_callback_t callback, void*cb_ptr);
// CALLBACK on packet FREE
_TRAPI transport_res_t transport_set_free_callback(transport_packet_t pkt, transport_free_callback_t callback, void*cb_ptr);
// CALLBACK on packet TRANSPORT_EVENT_DEBUG
_TRAPI transport_res_t transport_set_debug_event_callback(transport_point_t h, transport_request_callback_t callback, void*cb_ptr);


// Information functions
_TRAPI transport_pid_t transport_get_point_id(transport_point_t h); // get this point ID
_TRAPI const char* transport_get_point_name(transport_point_t h);

#define TRANSPORT_BUFFER_SIZE_DEFAULT	0 // set default system buffer size
#define TRANSPORT_BUFFER_SIZE_IGNORE	(-1) // ignore this value
_TRAPI transport_res_t transport_set_point_buffers_size(transport_point_t h, int send_sz, int recv_sz); // set socket send/recv buffer size, before point start
_TRAPI transport_pid_t transport_find_point_id_by_name(transport_point_t h, const char*dest_name); // find destination point ID
_TRAPI const char* transport_find_point_name_by_id(transport_point_t h, transport_pid_t pid); // find destination point name by ID
_TRAPI transport_pid_t transport_find_peer_id_by_name(transport_point_t h, const char*dest_name); // find destination point ID, only for peers
_TRAPI transport_point_t transport_get_point_handle(transport_packet_t packet); // get point associated with specified packet
_TRAPI transport_pid_t transport_get_source_id(transport_packet_t packet); // get source ID
_TRAPI transport_pid_t transport_get_target_id(transport_packet_t packet); // get target ID
_TRAPI transport_pid_t transport_get_remote_id(transport_packet_t packet); // get ID of remote point (source for incoming packets, target for outgoing)
_TRAPI const struct TRANSPORT_POINT_INFO*transport_get_point_info(transport_point_t h, transport_pid_t id); // get point info by ID
_TRAPI transport_eid_t transport_get_event_id(transport_packet_t packet); // get event ID
_TRAPI transport_time_t transport_get_packet_time(transport_packet_t packet); // returns 0 on invalid packet
_TRAPI transport_mid_t transport_get_message_id(transport_packet_t packet);
_TRAPI int transport_is_response(transport_packet_t packet);


#define TRANSPORT_PACKET_PRIORITY_NORMAL	0
#define TRANSPORT_PACKET_PRIORITY_HIGH		1

_TRAPI transport_res_t transport_set_packet_priority(transport_packet_t packet, int value);
_TRAPI transport_res_t transport_get_packet_priority(transport_packet_t packet, int *value);

#define TRANSPORT_SELECT_WORKING	1 // try to select working peer
#define TRANSPORT_SELECT_ANY		0 // select random peer

#define TRANSPORT_CHOOSE_RANDOM		0

_TRAPI transport_pid_t transport_select_peer(transport_point_t h, transport_pid_t group_id, unsigned flags); // select peer from group
_TRAPI transport_res_t transport_is_point_in_group(transport_point_t h, transport_pid_t group_id, transport_pid_t point_id); // returns TRANSPORT_OK if point_id is in group_id or point_id == group_id
_TRAPI transport_res_t transport_get_group_points(transport_point_t h, transport_pid_t group_id, transport_pid_t point_ids[], int*n_ids); // (*n_ids) should contains maximum allowed number of group points, receives actual number
_TRAPI transport_res_t transport_choose_cluster_node(transport_point_t h, transport_pid_t group_id, int ind); // ind = 0 - random, other vals = index of master point in cluster

#define TRANSPORT_SELECTOR_NONE		0 // disable packet selector
#define TRANSPORT_SELECTOR_FORCE	1 // force to use specified selector
#define TRANSPORT_SELECTOR_PREFER	2 // use specified selector if point is available

_TRAPI transport_res_t transport_set_packet_selector(transport_packet_t h, int value, unsigned flags); // value - any integer selector value

_TRAPI transport_time_t transport_get_local_time(); // get current local time
_TRAPI transport_time_t transport_get_point_time(transport_point_t h); // get point time (local time of last point operation)
_TRAPI transport_time_t transport_get_proc_time(); // always monotonic, not depends on local time adjustments
_TRAPI int transport_get_difftime_sec(transport_time_t t1, transport_time_t t2); // returns t2 - t1 in seconds
_TRAPI int transport_get_difftime_msec(transport_time_t t1, transport_time_t t2); // returns t2 - t1 in milliseconds
_TRAPI long long transport_get_difftime_usec(transport_time_t t1, transport_time_t t2); // returns t2 - t1 in microseconds
_TRAPI long long transport_get_difftime_nsec(transport_time_t t1, transport_time_t t2); // returns t2 - t1 in nanoseconds
_TRAPI time_t transport_time_to_time_t(transport_time_t t); // converts transport time to system time_t
_TRAPI transport_time_t transport_time_t_to_time(time_t t, unsigned nanosec); // convertes system time_t to transport time_t with nano-second accuracy
/*
	Format transport time:
		%H% - hour
		%M% - minute
		%S% - second
		%ms% - millisecond (fractional part of seconds)
		%us% - microseconds (fractional part of seconds)
		%ns% - nanoseconds (fractional part of seconds)
		%d% - day
		%m% - month
		%y%, %yyyy% - year (4-digit)
		%yy% - year (2-digit)
		
		%dd% - convert interval in days
		%dh% - convert interval in hours
		%dm% - convert interval in minutes
		%ds% - convert interval in seconds
		%dms% - convert interval in milliseconds
		%dus% - convert interval in microseconds
		%dns% - convert interval in nanoseconds
		
		%pt% - point name (if h != TRANSPORT_INVALID_HANDLE)
		
		%% - percent sign
*/
_TRAPI char* transport_format_time(transport_point_t h, transport_time_t t, const char*fmt, char*buf, int buflen);
_TRAPI char* transport_format_time_mask(transport_point_t h, const char*fmt, char*buf, int buflen);

struct TRANSPORT_VERSION_INFO
{
	int major, minor; // version
	int release; // revision
	int build; // build timestamp yyyymmdd
	int proto; // protocol version
};

_TRAPI const char* transport_get_version(struct TRANSPORT_VERSION_INFO*info); // info may be = NULL, returns string representation of version information

_TRAPI const char* transport_subsystem_id_to_name(int id);
_TRAPI int transport_subsystem_name_to_id(const char*name);

_TRAPI const char* transport_type_id_to_name(int t);
_TRAPI int transport_type_name_to_id(const char*name);

_TRAPI const char* transport_proto_id_to_name(int p);
_TRAPI int transport_proto_name_to_id(const char*name);

// helper functions for streaming data transfers
 _TRAPI transport_packet_t transport_create_streaming_response(transport_packet_t packet, transport_response_callback_t callback, void*cb_ptr); // create response packet


// helper functions for large data transfers

#define TRANSPORT_UNKNOWN_LENGTH (-1LL)
_TRAPI transport_packet_t transport_create_large_data_request(transport_point_t h, transport_pid_t target, transport_eid_t event, long long offset, long long length); // offset may be 0 (start), length may -1 (all data)

_TRAPI transport_res_t transport_suggest_buffer_size(transport_packet_t request, int len); // set suggested buffer size for remote side
_TRAPI int transport_get_suggested_buffer_size(transport_packet_t request); // get suggested buffer size from request

_TRAPI transport_packet_t transport_create_large_data_response(transport_packet_t request, transport_response_callback_t callback, void*cb_ptr); // create response packet with transfer callback
_TRAPI transport_res_t transport_push_large_data(transport_packet_t packet, const void*data, int len); // repeating function
_TRAPI transport_res_t transport_push_large_data_ex(transport_packet_t packet, const void*data, int len, struct TRANSPORT_ARRAY_INFO*ainf); // returns pointer to large data
_TRAPI transport_res_t transport_finish_large_transfer(transport_packet_t packet); // free packet resources and finish transfer

_TRAPI transport_large_offset_t transport_get_large_transfer_offset(transport_packet_t packet); // for request returns requested offset, for response - current offset
_TRAPI transport_large_length_t transport_get_large_transfer_length(transport_packet_t request);
_TRAPI transport_large_offset_t transport_get_large_transfer_remains(transport_packet_t response);

_TRAPI int transport_is_large_transfer_finished(transport_packet_t packet); // returns 0 if not, 1 if packet is the last packet
_TRAPI transport_res_t transport_get_large_data(transport_packet_t packet, struct TRANSPORT_ARRAY_INFO*info);

_TRAPI transport_packet_t transport_create_ping_packet(transport_point_t h, transport_pid_t target, const void*data, int dlen); // without setting error callback
_TRAPI transport_res_t transport_ping(transport_point_t h, transport_pid_t target, const void*data, int dlen, transport_error_callback_t error_callback, void*cb_ptr);

// EVENT loop for any transfers; delay_msec = 0 for check, (-1) for infinite loop
#define TRANSPORT_LOOP_NO_DELAY		0
#define TRANSPORT_LOOP_INFINITE_DELAY	(-1)
_TRAPI transport_res_t transport_loop(transport_point_t h, int delay_msec);
_TRAPI transport_res_t transport_loop_multi(transport_point_t h[], int n_points, int delay_msec);

_TRAPI transport_res_t transport_notify_loop(transport_point_t h); // notify on data arrived/changed to return from loop function immediately


#define TRANSPORT_SUSPEND_FOREVER (-1)
#define TRANSPORT_SUSPEND_RESUME  (0)
_TRAPI transport_res_t transport_suspend_point_receive(transport_point_t h, int msec); // sets/increments suspend time for specified point
_TRAPI transport_res_t transport_is_point_suspended(transport_point_t h); // TRANSPORT_OK if suspended


_TRAPI transport_res_t transport_suspend_stream(transport_packet_t req, int msec); // sets/increments suspend time for specified stream
_TRAPI transport_res_t transport_is_stream_suspended(transport_packet_t req); // TRANSPORT_OK if suspended


#define TRANSPORT_STAT_FORMAT_DEFAULT	0 // for transport_stat_config
#define TRANSPORT_STAT_FORMAT_PLAIN	1
#define TRANSPORT_STAT_FORMAT_XML	2
#define TRANSPORT_STAT_FORMAT_HTML	3
#define TRANSPORT_STAT_FORMAT_JSON	4

#define TRANSPORT_STAT_PERIOD_DEFAULT	(-1) // for transport_stat_config
#define TRANSPORT_STAT_FNAME_DISABLE	NULL // for transport_stat_config

enum {
	TRANSPORT_STAT_SENT, // number of sent messages
	TRANSPORT_STAT_RECEIVED, // number of received messages
	TRANSPORT_STAT_ALLOCATED, // number of allocated packets
	TRANSPORT_STAT_TO_SEND, // number of messages in to_send queue
	TRANSPORT_STAT_TO_ANSWER, // number of requests awaiting answers
	TRANSPORT_STAT_STREAM, // number of stream transfers in progress
	TRANSPORT_STAT_LARGE, // number of large transfers in progress
	TRANSPORT_STAT_DISCONNECTS, // number of disconnects for connection-oriented protocols
	TRANSPORT_STAT_CACHED, // number of cached messages
	TRANSPORT_STAT_SUSPENDED, // number of suspended messages
	TRANSPORT_STAT_INPROGRESS, // number of requests being processed
	TRANSPORT_STAT_DROPPED, // number of dropped packets (because of timeout or callback)
	TRANSPORT_STAT_REUSED, // number of reused packets (automatically or by the call to reuse_packet)
	
	TRANSPORT_N_STAT_COUNTERS = 16// total number of stat counters
};


#define POINT_STATE_WORKING	1
#define POINT_STATE_STOPPED	2

#define PEER_STATE_CONNECTING	1
#define PEER_STATE_CONNECTED	2
#define PEER_STATE_DISCONNECTED	3
#define PEER_STATE_REDUCED	4

#define HOST_STATE_CONNECTING	1
#define HOST_STATE_CONNECTED	2
#define HOST_STATE_DISCONNECTED	3

// collect statistical information to a packet
_TRAPI transport_res_t transport_append_stat_packet(transport_point_t h, transport_packet_t pkt);
// buf should be large enough to accept data in the required format
_TRAPI transport_res_t transport_stat_format_buffer(transport_packet_t pkt, int format, char*buffer, int *buflen);
_TRAPI transport_res_t transport_stat_clear(transport_point_t h[], int n_points);
_TRAPI transport_res_t transport_stat_get_config(transport_point_t h, int *format, const char**fname, int *period_msec);
_TRAPI transport_res_t transport_stat_set_config(transport_point_t h, int format, const char*fname, int period_msec);


// extra handles control
typedef transport_flags_t (*transport_handle_callback_t)(void*ptr, int handle, int event, transport_flags_t flags);

#define TRANSPORT_HANDLE_EVENT_PREPARE	1 // prepare handle to select, return desired handle flags (0 = ignore handle), flags = all possible flags for this platform
#define TRANSPORT_HANDLE_EVENT_PROCESS	2 // process select results
#define TRANSPORT_HANDLE_EVENT_HANGUP	3 // got hangup on socket, poll only
#define TRANSPORT_HANDLE_EVENT_TIMEOUT	4 // got timeout on socket, if was requested
#define TRANSPORT_HANDLE_EVENT_ERROR	(-1) // got error on socket, poll only
#define TRANSPORT_HANDLE_EVENT_BAD	(-2) // bad socket handle, poll only


#define TRANSPORT_HANDLE_FLAG_READ	1 // check/ready to read
#define TRANSPORT_HANDLE_FLAG_WRITE	2 // check/ready to write
#define TRANSPORT_HANDLE_FLAG_PRI	4 // check/ready to get priority data (if supported by handle, poll only)
#define TRANSPORT_HANDLE_FLAG_TIMEOUT	8 // enable timeout event
#define TRANSPORT_HANDLE_FLAG_EXCEPT	16 // enable extra event (if supported, select only)
#define TRANSPORT_HANDLE_FLAG_HUP	32 // hangup occured too (may be used for example with EVENT_ERROR)

_TRAPI transport_res_t transport_handle_control(transport_point_t h, int handle, transport_handle_callback_t callback, void*cb_ptr); // callback = NULL -> remove handle from table



/*
	General procedure for sending/receiving messages

	transport_set_error_callback(NULL, default_error_callback, NULL);
	transport_point_t h = transport_initialize_from_config("/opt/norsi/config/this_module.cfg");
	if (!h) { ... }
	transport_set_error_callback(h, some_error_callback, some_error_callback_arg);
	transport_set_request_callback(h, some_request_callback, some_request_callback_arg);
	
	for (;;) {
		transport_res_t res = transport_loop(h, TRANSPORT_LOOP_INFINITE_DELAY); // update events, may be used from separate thread
		if (res < 0) { ...; break; }
		if (some_data_ready) {
			transport_packet_t pkt = transport_create_packet(h, SOME_TARGET_POINT_ID, SOME_EVENT_ID);
			if (!pkt) { ... }
			transport_add_integer(pkt, SOME_DATA_ID, data_val);
			transport_add_string(pkt, SOME_STRING_ID, "test", -1);
			transport_set_response_callback(pkt, some_response_callback, some_response_callback_arg);
			res = transport_send_packet(pkt);
			if (res != TRANSPORT_OK) { ... }
		}
	}
	transport_terminate(h);

*/

/*
	Typical request handler
transport_res_t some_request_handler(void*ptr, transport_packet_t request)
{
	transport_packet_t response = transport_create_response(request);
	int arg = transport_get_integer(request, SOME_DATA_ID, 0);
	int res = compute_result(arg);
	transport_add_integer(response, SOME_RESULT_ID, res);
	transport_send_packet(response);
}
*/

/*
	Typical large transfer scheme
	Initiator:
	
	transport_packet_t req = transport_create_large_data_request(h, SOME_TARGET_ID, LARGE_REQUEST_EVENT_ID, transport_new_transfer_id(h), 0, 1000);
	transport_set_response_callback(req, transfer_function, some_ptr);
	transport_send_packet(req);
	...
	wait_some_event(_ev);
	
	transfer_function:
	
void transfer_function(void*ptr, transport_packet_t request, transport_packet_t response)
{
	long long ofs = transport_get_large_transfer_offset(response);
	struct TRANSPORT_ARRAY_INFO info;
	transport_get_large_data(response, &info);
	if (transport_is_large_transfer_finished(response)) {
		pulse_some_event(_ev);
	} else {
		write_some_data(ptr, info.data, info->length * info->el_size);
	}
}
	
	Responder:
transport_res_t request_callback(transport_packet_t request)
{
	if (transport_get_event_id(request) == LARGE_REQUEST_EVENT_ID) {
		transport_packet_t response;
		response = transport_create_large_data_response(request, transfer_callback, transfer_ptr);
		if (!response) { ... }
		... add some custom fields to response
	}
	return TRANSPORT_OK;
}

transport_res_t transfer_callback(void*ptr, transport_packet_t request, transport_packet_t response)
{
	if (transport_get_large_transfer_remains(response) > 0) {
		int len = read_some_data(ptr, some_buf);
		return transport_push_large_data(response, some_buf, len);
	} else return transport_finish_large_transfer(response); // no need to free request
}
*/


/*
	Typical streaming transfer scheme:
transport_res_t handle_streaming_request(transport_packet_t request)
{
	transport_packet_t response = transport_create_streaming_response(request, transfer_callback, transfer_ptr);
	if (!response) { ... }
	... add some required fields
	transport_add_array(response, SOME_ARRAY_ID, buffer_size, NULL); // prepare space for streaming data
	... add some other fields
	return TRANSPORT_OK;
}

transport_res_t transfer_callback(void*ptr, transport_packet_t request, transport_packet_t response)
{
	if (is_finished) {
		transport_free_packet(response); // no need to free request
		return TRANSPORT_OK;
	}
	if (has_data) {
		struct TRANSPORT_ARRAY_INFO info;
		transport_get_array_info(response, SOME_ARRAY_ID, &info);
		do_read_data_for_streaming(info.data, info.length * info.el_size); // fill array with data
		return TRANSPORT_OK; // no need to push packet
	} else return TRANSPORT_NO_DATA;
}
*/


#ifdef __cplusplus
} // extern "C"
#endif


#endif // TRANSPORT_H
