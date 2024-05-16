#include "module.h"

extern int module_num;


static int configure_points(struct SERVER_STATE*ss, struct MODULECONF*mc)
{
        if (!strcmp(ss->sc.target_point, "none")) {
                WARN(NOTIFY, "Модуль запущен без точки-получателя");
                ss->target_id = TRANSPORT_INVALID_POINT_ID;
        } else {
                ss->target_id = transport_find_point_id_by_name(ss->tr, ss->sc.target_point);
                if (ss->target_id == TRANSPORT_INVALID_POINT_ID) {
                        ERROR(5, "Невозможно найти идентификатор точки %s", ss->sc.target_point);
                        return -1;
                }
        }

        return 0;
}

static int configure_transport(struct SERVER_STATE*ss){
	transport_res_t r;
	if(!ss->sc.transport_point[0]){
        WARN(IMPORTANT, "в настройках не указана транспортная точка, транспортная подсистема отключена");
        ss->tr = TRANSPORT_INVALID_HANDLE;
        return 0;
	}
    INFO(EXTRA, 0, "выполняется инициализация транспортной подсистемы для точки %s из файла %s", ss->sc.transport_point, ss->sc.transport_config);
    INFO(NOTIFY, 0, "Транспортная библиотека: %s", transport_get_version(NULL));

    ss->tr = transport_initialize_from_config(ss->sc.transport_config, ss->sc.transport_point, TRANSPORT_CONFIG_DEFAULT);
    if (!ss->tr) {
            ERROR(-60, "невозможно выполнить инициализацию транспорта для точки %s", ss->sc.transport_point);
            return -1;
    }
    r = transport_start_point(ss->tr, 0);

    if (TRANSPORT_FAILED(r)) {
            ERROR(-63, "невозможно запустить точку %s", ss->sc.transport_point);
            return -1;
    }
    INFO(EXTRA, 0, "транспортная подсистема инициализирована");
    return 0;
}

static int term_transport(struct SERVER_STATE*ss){
	if(ss->tr){
		transport_terminate(ss->tr);
		INFO(EXTRA,0, "transport system is stoped");
	}
	return 0;
}

int read_conf(struct SERVER_STATE* ss, const struct MODULECONF*mc){

	const struct CFG_NODE*node;
	node = cfgparser_findnode_root(mc->config,"config");
	if (!node) { ERROR(-1, "no config group found!"); return -1; }

	ss->sc.upd_interval_sec = cfgparser_get_int(node,"upd_interval_sec", 1000);
	ss->sc.loop_ms = cfgparser_get_int(node,"loop_ms",100);
	ss->sc.port = cfgparser_get_int(node,"port",7000);
	ss->sc.backlog = cfgparser_get_int(node,"backlog",10);
	ss->sc.target_timeout_ms = cfgparser_get_int(node, "target_timeot_ms", 1000);
	ss->sc.timeout_fopen_err_ms = cfgparser_get_int(node,"timeout_fopen_err_ms",5000);
	strncpy(ss->sc.module_info,cfgparser_get_str(node,"module_info",""), sizeof(ss->sc.module_info) - 1);
	snprintf(ss->sc.transport_point,sizeof(ss->sc.transport_point),cfgparser_get_str(node,"point_name",POINT_NAME),module_num);
	snprintf(ss->sc.target_point,sizeof(ss->sc.transport_point),cfgparser_get_str(node,"target_name",POINT_NAME),module_num);

    strncpy(ss->sc.transport_config,cfgparser_get_str(node,"transport",""), sizeof(ss->sc.transport_config));
    strncpy(ss->sc.pto_config,cfgparser_get_str(node,"ptoconf",""),sizeof(ss->sc.pto_config));

	DEBUG(NOTIFY,"transport config %s",ss->sc.transport_config);
	DEBUG(NOTIFY,"pto config %s",ss->sc.pto_config);

	return 0;
}

int module_initialize(struct SERVER_STATE*ss, const char* fname, int def_debug){

	ss->def_debug = def_debug? def_debug : PTO_LEVEL_IMPORTANT;
	strncpy(ss->module_conf_name, fname, sizeof(ss->module_conf_name) - 1);
	pto_init(NULL, ss->def_debug);
	INFO(NOTIFY, CODE_COMMON_INFO_INTRMODINIT, "Инициализация модуля %s %s-%d build %d (процесс %d)...", MODULE_NAME, MODULE_VERSION, MODULE_RELEASE, MODULE_BUILD, getpid());
    INFO(NOTIFY, CODE_COMMON_INFO_SUCMODINIT, "Инициализация модуля выполнена");
    return 0;

}

int module_configure(struct SERVER_STATE* ss, int argc, char* const argv[]){

	int r;
	struct MODULECONF mc;
	module_conf_clear(&mc);

	r = module_conf_load(&mc, ss->module_conf_name);
	if(r) goto err1;

	r = read_conf(ss,&mc);
	if(r) goto err;

	opterr = 0;
	optind = 1;
    while ((r = getopt(argc, argv, "c:t:i:P:p:d::g::l:e::I:T:N:")) != -1)  {
            switch (r) {
            case 't': strncpy(ss->sc.transport_config, optarg, sizeof(ss->sc.transport_config) - 1); break;
            case 'p': strncpy(ss->sc.transport_point, optarg, sizeof(ss->sc.transport_point) - 1); break;
            case 'P': strncpy(ss->sc.pto_config, optarg, sizeof(ss->sc.pto_config) - 1); break;
            case 'i': strncpy(ss->sc.module_info, optarg, sizeof(ss->sc.module_info) - 1); break;
            //case 'I': strncpy(ss->gconf.interface, optarg, sizeof(ss->gconf.interface) - 1); break;
            case 'T': strncpy(ss->sc.target_point, optarg, sizeof(ss->sc.target_point) - 1); break;
            //case 'N': ss->sc.base_num = strtoul(optarg, NULL, 0); break;
            case '?': fprintf(stderr, "error: unknown argument %c\n", optopt); return -1;
            }
    }

    r = configure_transport(ss);
    if (r) goto err;

    pto_init(ss->tr, ss->def_debug);

    if (!ss->sc.pto_config[0]) {
            r = pto_configure_from_file(ss->sc.pto_config, NULL);
            if (r) goto err;
    }

    r = configure_points(ss, &mc);

    if (r) goto err;

    module_conf_free(&mc);

    INFO(NOTIFY, CODE_COMMON_INFO_SUCMODCONF, "Завершено конфигурирование модуля (процесс %d)", getpid());
    return 0;

err:
	module_conf_free(&mc);

err1:
	INFO(IMPORTANT, CODE_COMMON_INFO_INTRMODINIT, "Настройка модуля прервана с кодом ошибки %d", r);

	return r;
}

void module_terminate(struct SERVER_STATE* ss){
    INFO(NOTIFY, CODE_COMMON_INFO_STOPMODREG, "Остановка модуля (процесс %d, точка %s)...", getpid(), ss->sc.transport_point);
    INFO(NOTIFY, CODE_COMMON_INFO_SUCSTOPMOD, "Модуль остановлен");
    //if (state->tr) transport_stop_point(state->tr);
    queue_destroy(ss->queue);
    term_transport(ss);
	pto_term();
}
