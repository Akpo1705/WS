

/*
	List of transport return codes
*/

#define TRANSPORT_SUSPEND	2    // special code for callbacks, suspend stream and recall this packet after resume
#define TRANSPORT_FINISH	1    // special code for callbacks, finish stream
#define TRANSPORT_OK		0    // success
#define TRANSPORT_ERROR		(-1) // general error
#define TRANSPORT_INV_PARAM	(-2) // invalid parameter
#define TRANSPORT_IO_FAILURE	(-3) // i/o failure
#define TRANSPORT_NO_MEMORY	(-4) // memory allocation error
#define TRANSPORT_NO_SPACE	(-5) // disk space error
#define TRANSPORT_NO_DATA	(-6) // data not found
#define TRANSPORT_NOT_IMPL	(-7) // not implemented
#define TRANSPORT_INV_PROTO	(-8) // protocol failure
