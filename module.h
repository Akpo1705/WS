#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <assert.h>

#include "include/transport/transport.h"
#include "include/pto-iface/pto_iface.h"
#include "include/cfgparser/processconf.h"
#include "moduleconfparser.h"
#include "common_defines.h"
#include "config.h"
#include "release.h"
#include "networking/server.h"
#include "networking/HTTPRequest.h"
#include "hash-dict/dict.h"
#include "queue/queue.h"
#include "parser_uri/parser_uri.h"

#define WS_VERSION 13
#define WS_WEBSOCK "websocket"
#define WS_MOBILE "?0"
#define WS_MAGIC   "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

#define MAX_FNAME 255
#define MAX_STR 128
#define TRUE 1
#define FALSE 0

struct ModuleState{
	char* module_config;
	char* transport_config;
};

enum wsFrameType {
    WS_EMPTY_FRAME 				= 0xF0,
    WS_ERROR_FRAME 				= 0xF1,
    WS_INCOMPLETE_FRAME 	= 0xF2,
    WS_TEXT_FRAME 					= 0x01,
    WS_BINARY_FRAME 				= 0x02,
    WS_PING_FRAME 					= 0x09,
    WS_PONG_FRAME 				= 0x0A,
    WS_OPENING_FRAME 			= 0xF3,
    WS_CLOSING_FRAME 			= 0x08
};

enum wsState {
    CONNECTING 	= 0x00,  /*The connection is not yet open.*/
    OPEN 		= 0x01,  /*The connection is open and ready to communicate.*/
    CLOSING 	= 0x02,  /*The connection is in the process of closing.*/
    CLOSED 		= 0x03,  /*The connection is closed or couldn't be opened.*/
	KEEP_ALIVE 	= 0x04,
};

struct ws_frame {
    uint8_t fin;
    uint8_t rsv1;
    uint8_t rsv2;
    uint8_t rsv3;
    uint8_t opcode;
    uint8_t *payload;
    uint64_t payload_length;
    enum wsFrameType type;
};


#define TIME_COUNT 7
typedef struct TABLE_SR{	// информация о повторной строке из другой таблицы
	int index_tbl;
	int index_row;
	int cmp_ofs;

	int msec_duration;
	int sec_duration;

	int start[TIME_COUNT];
	int ans[TIME_COUNT];
}tbl_sr, *tbl_sr_t;

typedef int avl_type_t;
typedef struct avl_tree{        // узел для дерева
        avl_type_t key;

        tbl_sr** sr;
        int count_sr;
        int len_sr;

        int height;
        struct avl_tree *left;
        struct avl_tree *right;
}avl_node_t, *avl_tree_t;

struct SERVER_CONF{
	char 	module_info[MAX_FNAME];
	char 	loop_dir[MAX_FNAME];
	char 	transport_config[MAX_FNAME];
	char 	transport_point[MAX_STR];
	char 	target_point[MAX_STR];
	char 	pto_config[MAX_FNAME];

	int loop_ms;
	int target_timeout_ms;
	int upd_interval_sec;
	int timeout_fopen_err_ms;

	int port;
	int backlog;
};

#define WS_HDR_TEXT_HTML 		"text/html"
#define WS_HDR_TEXT_CSS 		"text/css"
#define WS_HDR_APPLICATION_JS 	"application/js"
#define WS_HDR_IMAGE_JPEG 		"image/jpeg"
#define WS_HDR_IMAGE_PNG 		"image/png"
#define WS_HDR_IMAGE_GIF 		"image/gif"

#define WS_HDR_CON_KEEP_ALIVE 	"keep-alive"
#define WS_HDR_CON_CLOSE 		"close"

#define WS_HDR_KEY 				"Sec-WebSocket-Key"
#define WS_HDR_VER 				"Sec-WebSocket-Version"
#define WS_HDR_ACP 				"Sec-WebSocket-Accept"
#define WS_HDR_ORG 				"Origin"
#define WS_HDR_HST 				"Host"			/*Определяет хост и порт, к которому отправляется запрос. Этот заголовок необходим, когда на одном сервере работает несколько веб-сайтов.*/
#define WS_HDR_UPG 				"Upgrade"
#define WS_HDR_CON 				"Connection"		/*Управляет параметрами соединения, такими как "keep-alive" для повторного использования TCP-соединения.*/
#define WS_HDR_UAG 				"User-Agent"		/*Содержит информацию о браузере или клиенте, отправившем запрос.*/
#define WS_HDR_COT 				"Content-type"	/*Указывает тип данных, передаваемых в теле запроса или ответа (например, "application/json", "text/html", "image/jpeg" и т. д.).*/
#define WS_HDR_COL 				"Content-length" /*Содержит длину тела запроса или ответа в байтах*/
#define WS_HDR_ACC 				"Accept" 		/*Определяет типы медиа-ресурсов, которые клиент готов принимать*/
#define WS_HDR_AUTH 			"Authorization" /*Передает данные аутентификации для доступа к защищенным ресурсам.*/
#define WS_HDR_COOKIE 			"Cookie" 		/*Передает информацию о сеансе и сессионных данными между клиентом и сервером.*/
#define WS_HDR_LOCATION 		"Location"	/*Используется в HTTP-редиректах для указания нового местоположения ресурса.*/
#define WS_HDR_CACHE_CONTROL 	"Cache-Control" /*Управляет кэшированием содержимого на клиенте или сервере.*/
#define WS_HDR_ETag 			"ETag" /*Содержит уникальный идентификатор версии ресурса, который используется для проверки условных запросов.*/
#define WS_HDR_IF_MODIFI 		"If-Modified-Since" /*Позволяет серверу проверить, был ли ресурс изменен после указанной даты.*/
#define WS_HDR_ACC_ENC 			"Accept-Encoding"
#define WS_HDR_SET_COOKIE 		"Set-Cookie"
#define WS_HDR_CORS 			"Access-Control-Allow-Origin" /*Разрешает доступ к ресурсу из другого источника (CORS).*/
#define WS_HDR_MOBILE 			"sec-ch-ua-mobile" /*указывает на то, что браузер находится на мобильном устройстве*/
#define WS_HDR_FETCH_MODE 		"Sec-Fetch-Mode"
#define WS_HDR_FETCH_DEST 		"Sec-Fetch-Dest"
#define WS_HDR_FETCH_SITE 		"Sec-Fetch-Site"
#define WS_HDR_ACCEPT_LANGUAGE 	"Accept-Language"
#define WS_HDR_DATE 			"Date"

enum wsFetchMode{
	CORS_MODE			= 0x00, /*Запрос является запросом по протоколу CORS.*/
	NAVIGATE_MODE		= 0x01, /*Запрос инициируется навигацией между HTML-документами.*/
	NOCORS_MODE			= 0x02, /*Запрос является no-cors запросом (см. Request.mode).*/
	SAME_ORIGIN_MODE 	= 0x03, /*Запрос выполняется из того же источника, что и запрашиваемый ресурс.*/
	WEBSOCKET_MODE 		= 0x04, /*Выполняется запрос на установление WebSocket-соединения.*/
};

enum wsFetchDest{
	AUDIO 			= 0, /*Местом назначения являются аудиоданные. Это может происходить из HTML-тега <audio>.*/
	AUDIOWORKLET 	= 1, /*Местом назначения являются данные, получаемые для использования рабочим аудиомодулем. Это может произойти в результате вызова audioWorklet.addModule().*/
	DOCUMENT 		= 2, /*Местом назначения является документ (HTML или XML), а запрос - результат инициированной пользователем навигации верхнего уровня (например, в результате перехода по ссылке).*/
	EMBED 			= 3, /*Местом назначения является встроенное содержимое. Это может происходить из тега HTML <embed>.*/
	EMPTY 			= 4, /*Место назначения представляет собой пустую строку. Используется для пунктов назначения, которые не имеют собственного значения. Например, fetch(), navigator.sendBeacon(), EventSource, XMLHttpRequest, WebSocket и т.д.*/
	FONT 			= 5, /*Местом назначения является шрифт. Это может происходить из CSS @font-face*/
	FRAME 			= 6, /*Местом назначения является фрейм. Он может возникнуть из тега HTML <frame>.*/
	IFFRAME 		= 7, /*Местом назначения является iframe. Он может возникнуть из тега HTML <iframe>.*/
	IMAGE 			= 8, /*Местом назначения является изображение. Оно может исходить из HTML <image>, SVG <image>, CSS background-image, CSS cursor, CSS list-style-image и т.д.*/
	MANIFEST 		= 9, /*Местом назначения является манифест. Это может происходить из HTML <link rel=manifest>.*/
	OBJECT 			= 10,/*Местом назначения является объект. Он может исходить из тега HTML <object>.*/
	PAINTWORKLET 	= 11,/*Местом назначения является рабочий модуль рисования. Он может возникнуть в результате вызова CSS.PaintWorklet.addModule().*/
	REPORT 			= 12,/*Местом назначения является отчет (например, отчет о политике безопасности содержимого).*/
	SCRIPT  		= 13,/*Местом назначения является скрипт. Он может быть получен из HTML-тега <script> или вызова функции WorkerGlobalScope.importScripts().*/
	SERVICEWORKER 	= 14,/*Местом назначения является рабочий сервис. Это может произойти от вызова navigator.serviceWorker.register().*/
	SHAREDWORKER 	= 15,/*Местом назначения является общий рабочий. Он может исходить от SharedWorker.*/
	STYLE 			= 16,/*Местом назначения является стиль. Он может быть получен из HTML <link rel=stylesheet> или из CSS @import.*/
	TRACK 			= 17,/*Местом назначения является текстовая дорожка HTML. Она может исходить из тега HTML <track>.*/
	VIDEO 			= 18,/*Местом назначения являются видеоданные. Это может происходить из тега HTML <video>.*/
	WORKER 			= 19,/*Местом назначения является Worker*/
	XSLT 			= 20,/*Местом назначения является XSLT-преобразование.*/
	MEDIA = 21,

};

enum wsFetchSite{
	CROSSSITE 	= 0, /*Инициатор запроса и сервер, на котором размещен ресурс, имеют разные сайты (например, запрос от "potentially-evil.com" к ресурсу на "example.com").*/
	SAMEORIG 	= 1, /*Инициатор запроса и сервер, на котором размещен ресурс, имеют одинаковое происхождение (одинаковые схема, хост и порт).*/
	SAMESITE 	= 2, /*Инициатор запроса и сервер, размещающий ресурс, имеют одну и ту же схему, домен и/или субдомен, но не обязательно один и тот же порт.*/
	NONE 		= 3, /*Этот запрос является операцией, инициированной пользователем. Например, ввод URL-адреса в адресную строку, открытие закладки или перетаскивание файла в окно браузера.*/
};

enum wsHTTPMethods
{
    CONNECT,
    DELETE,
    GET,
    HEAD,
    OPTIONS,
    PATCH,
    POST,
    PUT,
    TRACE
};

struct http_header{
	unsigned char version;
	unsigned char upgrade;
	unsigned char webcocket;
	unsigned char mobile;

	char method[4];
	char key[32];
	char host[32];
	char accept_encoding[64];
	char accept_lang[64];
	char uri[128];
	char accept[256];
	char user_agent[256];

	enum wsFrameType type;
	enum wsState state;
	enum wsFetchMode fetchMode;
	enum wsFetchDest fetchDest;
	enum wsFetchSite fetchSite;
	enum wsHTTPMethods wsMethod;
};


#define BUF_LEN 2048
struct fds {
    int fd;
    uint8_t buffer[BUF_LEN];
    enum wsState state;
    struct ws_frame fr;
    int readed;
    int readedLength;
};


#define MAX_FD	1024
struct SERVER_STATE{
	struct SERVER_CONF sc;

	struct Server server;
	unsigned int addrlen;
	pid_t pid;
	int new_socket;
	int fd[2];
	char *hello;

	FILE* test_fd;

	fd_set readfds;
	struct fds client_socket[MAX_FD];

	char module_conf[MAX_FNAME];
	int def_debug;
	int stop;
	int update;
	char module_conf_name[MAX_FNAME];

	queue_t* queue;

	transport_point_t tr;
	transport_time_t cur_ts, upd_ts;
	transport_time_t speed_ts;
	transport_time_t module_loop_time;
	transport_time_t start_t, start_ts;
	transport_pid_t target_id;
};

void module_clear(struct SERVER_STATE *ss);
int module_initialize(struct SERVER_STATE*ss, const char* fname, int def_debug);
void module_terminate(struct SERVER_STATE* ss);
int module_loop(struct SERVER_STATE* ss);
int module_listen_loop(struct SERVER_STATE* ss);
int test_module_loop(struct SERVER_STATE* ss);
int module_configure(struct SERVER_STATE*state, int argc, char* const argv[]);

void server_init(struct SERVER_STATE* ss);

avl_node_t* avltree_search(avl_tree_t tree, avl_type_t key);
avl_node_t* avltree_insert(avl_tree_t tree, avl_type_t key);
avl_node_t* avltree_delete(avl_tree_t tree, avl_type_t key);
void avltree_destory(avl_tree_t tree);
void avltree_show(avl_tree_t tree, avl_type_t key, int direction);
void avltree_traverse_preorder(avl_tree_t tree);
int avltree_add_data(avl_tree_t avl_tree, tbl_sr_t tbl_sr);



char* ws_handshake(struct http_header *header, uint8_t *in_buf, int in_len, int *out_len);
