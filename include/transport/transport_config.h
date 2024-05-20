#ifndef TRANSPORT_CONFIG_H
#define TRANSPORT_CONFIG_H

struct TRANSPORT_HOST_INFO
{
	char name[TRANSPORT_MAX_STR]; // human-readable host name
	int  host_type;
	union {
		struct { // for HOST_TYPE_REMOTE
			char address[TRANSPORT_MAX_STR];
			int  port;
		};
		struct { // for HOST_TYPE_LOCAL
			int token;
		};
		//...
	};
};

struct TRANSPORT_POINT
{
	char name[TRANSPORT_MAX_STR]; // human-readable endpoint name
	transport_pid_t id; // internal endpoint identifier
};

struct TRANSPORT_HOST_REF
{
	int host_ind;
	union {
		int	replacement_port;
		int	replacement_token;
	};
};

struct TRANSPORT_POINT_REF
{
	int point_ind;
};


struct TRANSPORT_POINT_INFO
{
	struct TRANSPORT_POINT id;
	int	point_type;
	int	group_size;
	union {
		struct TRANSPORT_HOST_REF host_ref;
		struct TRANSPORT_POINT_REF *point_refs; // for clusters/broadcasts
	};
	int	n_peers;
	struct TRANSPORT_POINT_REF *peer_refs; // peer indices in point array
};


#define TRANSPORT_STATISTICS_NONE	0
#define TRANSPORT_STATISTICS_TEXT	1
#define TRANSPORT_STATISTICS_XML	2
#define TRANSPORT_STATISTICS_HTML	3

struct TRANSPORT_COMMON_PARAMETERS
{
	int max_attempts_before_free; // for messages without guaranteed delivery, 0 = never free
	int max_attempts_before_cache; // for messages with guaranteed delivery, 0 = never cache
	int send_timeout_msec;
	int max_msec_before_cache; // like previous but depends on time, 0 = never cache
	int max_msec_before_retry; // for unacknowledged requests, 0 = never retry
	int max_retries_before_free; // 0 = never free unacknowledged messages
	int default_transfer_size; // default buffer size for large file transfers
	char default_cache_dir_name[TRANSPORT_MAX_FNAME]; // default directory name for cache
	int use_cache; // enable or disable the use of the disk cache
	int connect_msec_before_retry; // 0 - retry without delay
	int statistics_format;
	char statistics_file[TRANSPORT_MAX_FNAME];
	int statistics_period; // msec
	int suspended_period; // msec, interval between calling callback for suspended messages
	int update_period; // msec, interval to update packet timeouts and so on
	int cache_update_period; // msec, interval to update packets in the cache
	int default_ping_timeout; // msec, time for transport_ping function
	int default_pong_timeout; // msec, time for automatic PONG response
	int max_packets_to_free; // maximum count of packets in free queue (increase to simplify packet reuse)
	int data_granularity; // granularity of mallocs/reallocs for packets
	int max_suspended; // maximum number of suspended incoming messages before input socket became blocked
	int max_to_send; // maximum number of outgoing messages before input socket became blocked
	int max_to_reuse; // maximum size in bytes of packet to be reused (0 = reuse all packets, 1 = reuse no packets)
};


#define TRANSPORT_CONFIG_VERSION_1	1
#define TRANSPORT_CONFIG_VERSION TRANSPORT_CONFIG_VERSION_1

// no pointers, may be stored on a disk
struct TRANSPORT_CONFIG
{
	int  version;
	char name[TRANSPORT_MAX_STR]; // file name of configuration file
	struct TRANSPORT_COMMON_PARAMETERS common;
	int n_points;
	struct TRANSPORT_POINT_INFO *points;
	int	n_hosts;
	struct TRANSPORT_HOST_INFO *hosts;
};

// Compatibility definitions

#ifdef _FROM_LIB
#define transport_load_config_compat transport_load_config
#define transport_save_config_compat transport_save_config
#else
#ifdef _TRANSPORT_OLD_SYNTAX
#define transport_load_config_compat transport_load_config
#define transport_save_config_compat transport_save_config
#else
#define transport_load_config transport_load_config_ver2
#define transport_save_config transport_save_config_ver2
#endif
#endif


// parse specified configuration file
_TRAPI transport_res_t transport_load_config_ver2(struct TRANSPORT_CONFIG*cfg, const char*fname, unsigned flags);
_TRAPI transport_res_t transport_load_config_compat(struct TRANSPORT_CONFIG*cfg, const char*fname);
// export previously parsed configuration file back to text
_TRAPI transport_res_t transport_save_config_ver2(const struct TRANSPORT_CONFIG*cfg, const char*fname, unsigned flags);
_TRAPI transport_res_t transport_save_config_compat(const struct TRANSPORT_CONFIG*cfg, const char*fname);
// find point information in config
_TRAPI const struct TRANSPORT_POINT_INFO* transport_find_point_in_config_by_name(const struct TRANSPORT_CONFIG*cfg, const char*point_name); 
_TRAPI const struct TRANSPORT_POINT_INFO* transport_find_point_in_config_by_id(const struct TRANSPORT_CONFIG*cfg, transport_pid_t id); 
_TRAPI void transport_clear_config(struct TRANSPORT_CONFIG*cfg); // clear structure before any use
_TRAPI void transport_free_config(struct TRANSPORT_CONFIG*cfg); // free resources after load
_TRAPI transport_res_t transport_copy_config(struct TRANSPORT_CONFIG*dest, const struct TRANSPORT_CONFIG*src); // dest may be filled with data or should be clear

_TRAPI transport_res_t transport_scan_compatible_hosts(struct TRANSPORT_CONFIG*cfg, const char*iface, const char*addr_sub, int host_inds[], int*n_hosts);
_TRAPI transport_res_t transport_scan_compatible_points(struct TRANSPORT_CONFIG*cfg, const char*iface, const char*addr_sub, int point_inds[], int*n_points);


#endif //TRANSPORT_CONFIG_H
