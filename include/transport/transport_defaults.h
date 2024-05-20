
/*
	Some default configuration values
*/


#define MAX_ATTEMPTS_BEFORE_FREE	20 // for messages without guaranteed delivery
#define MAX_ATTEMPTS_BEFORE_CACHE	30 // for messages with guaranteed delivery
#define MAX_MSEC_BEFORE_CACHE		1000 // for messages with guaranteed delivery
#define MAX_MSEC_BEFORE_RETRY		5000 // for unacknowledged messages
#define MAX_RETRIES_BEFORE_FREE		0 // 0 - never free unacknowledged messages

#define DEFAULT_SEND_TIMEOUT		100 // send timeout before increment_failure occurs

#define DEFAULT_SUSPENDED_PERIOD	1000 // default period (ms) before retry to call callback for suspended messages

#define DEFAULT_TRANSFER_SIZE		4096 // for large transfers

#define DEFAULT_CACHE_DIR_NAME		"trcache"

#define DEFAULT_CONNECT_MSEC_BEFORE_RETRY	500

#define DEFAULT_STATISTICS_FORMAT	TRANSPORT_STATISTICS_XML
#define DEFAULT_STATISTICS_PERIOD	3000
#define DEFAULT_STATISTICS_FILE		"trstats.xml"

#define DEFAULT_UPDATE_PERIOD		300 // msec
#define DEFAULT_CACHE_UPDATE_PERIOD	500 // msec

#define DEFAULT_PING_TIMEOUT		5000 // msec
#define DEFAULT_PONG_TIMEOUT		10000 // msec

#define MAX_PACKETS_TO_FREE		256

#define DEFAULT_DATA_GRANULARITY	256 // bytes

#define DEFAULT_MAX_SUSPENDED		1024 // messages
#define DEFAULT_MAX_TO_SEND		10000 // messages

#define DEFAULT_MAX_TO_REUSE		32768 // bytes
