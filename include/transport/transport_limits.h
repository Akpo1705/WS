

/*
	List of general transport limit constants
*/

#define TRANSPORT_MAX_STR	128 // generic string
#define TRANSPORT_MAX_FNAME	1024 // generic file name
#define TRANSPORT_MAX_GROUP_SIZE	4096 // maximum size of host group
#define TRANSPORT_MAX_POINTS_COUNT	8192 // maximum number of points for single module
#define TRANSPORT_MAX_HOSTS_COUNT	8192 // maximum number of hosts for single module
#define TRANSPORT_MAX_SUBSYSTEMS	64
#define TRANSPORT_MAX_PEERS		8192 // maximum number of peers for single point
#define TRANSPORT_MAX_SMALL_MESSAGE_SIZE	4096 // maximum size of small messages (via MQ or UDP), OS may establish more strict limits
#define TRANSPORT_DEFAULT_TRANSFER_BUFFER_SIZE	4096 // default transfer buffer size for large file transfers
#define TRANSPORT_MAX_EXTRA_HANDLES		1024 // maximum number of extra handles
#define TRANSPORT_MAX_STRUCT_LEVEL	16 // maximum level of structures to begin/end
#define TRANSPORT_MAX_ENTER_LEVEL	16 // maximum level of structures to enter/leave
