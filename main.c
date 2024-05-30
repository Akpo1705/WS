#include "module.h"

struct SERVER_STATE ss;
int def_debug = PTO_LEVEL_DEVEL;
int is_daemon = 0;
int real_daemon = 0;
int guarded = 0;
int module_num = 1;
char config_name[1024] = "module_config";

#define MAIN_CONF "webserver.conf"

int parse_common_args(int argc, char *argv[]) {
	int i;
	for (i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'c':
				strncpy(config_name, argv[i] + 2, sizeof(config_name) - 1);
				break;
			case 'l':
				def_debug = strtol(argv[i] + 2, NULL, 0);
				break;
			case 'd':
				if (argv[i][2])
					is_daemon = strtol(argv[i] + 2, NULL, 0);
				else
					is_daemon = 1;
				break;
			case 'g':
				if (argv[i][2])
					guarded = strtol(argv[i] + 2, NULL, 0);
				else
					guarded = 1;
				break;
				//case 'h': module_help(&ms, argv[0]); exit(5);
				//case 'v': module_version(&ms, argv[0]); exit(6);
			}
		} else {
			char *ep;
			module_num = strtoul(argv[i], &ep, 0);
			if (*ep) {
				fprintf(stderr, "unable to parse module number at %s\n", ep);
				exit(4);
			}
		}
	}
	return 0;
}

void sighandler(int sig) {
	printf("exit\n");
	//SIG_DFL обработка сигнала по умолчанию
	if (sig == SIGTERM && ss.stop)
		signal(SIGTERM, SIG_DFL);
	ss.stop = 1;
}

void updhandler(int sig) {
	if (ss.stop)
		return;
	ss.update = 1;
}

int main(int argc, char *argv[]) {

	int r;
	module_clear(&ss);
	//pid_t   childpid;

	char *name_test_file = "test_packet.txt";

	ss.test_fd = fopen(name_test_file, "r");
	if (ss.test_fd == NULL) {
		DEBUG(DEVEL, "can't open test file packet....");
		exit(1);
	}

	parse_common_args(argc, argv);
	ss.def_debug = PTO_LEVEL_DEVEL;
	r = module_initialize(&ss, MAIN_CONF, def_debug);

	if (r)
		return 1;

	if (is_daemon) {
		DEBUG(DEVEL, "going daemon...");
		r = daemon(0, 0);
		if (r) {
			ERROR(3, "unable to became daemon!");
			return 3;
		}
		real_daemon = 1;
	}

	signal(SIGTERM, sighandler);
	signal(SIGHUP, updhandler);
	signal(SIGINT, sighandler);
	r = module_configure(&ss, argc, argv);

	if (pipe(ss.fd) < 0)
		exit(1);

	server_init(&ss);
//    ss.addrlen = sizeof(ss.server.address);

	test_module_loop(&ss);

	//while(!ss.stop){
	//module_listen_loop(&ss);
	//module_loop(&ss);
	//}
	//kill(server_state.pid , SIGKILL);
	module_terminate(&ss);

	return 0;
}
