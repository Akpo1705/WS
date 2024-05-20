#define CODE_COMMON_OK 	0 // Успех
#define CODE_COMMON_INFO_STARTMODINIT 	1 // начата инициализация модуля
#define CODE_COMMON_INFO_SUCMODINIT 	2 // инициализация успешно завершена
#define CODE_COMMON_INFO_INTRMODINIT 	3 // инициализация прервана, модуль остановлен
#define CODE_COMMON_INFO_STOPMODREG 	11 // начата плановая остановка модуля
#define CODE_COMMON_INFO_SUCSTOPMOD 	12 // остановка модуля успешно завершена
#define CODE_COMMON_INFO_STOPMODERR 	13 // остановка модуля завершена в аварийном режиме (по запросу)
#define CODE_COMMON_INFO_INTRMODSTOP 	14 // остановка модуля прервана
#define CODE_COMMON_INFO_STOPMOD 	15 // аварийная остановка модуля (без запроса)
#define CODE_COMMON_INFO_FREEMODRES 	16 // освобождение ресурсов модуля
#define CODE_COMMON_INFO_GOTSIGNAL 	17 // получен сигнал
#define CODE_COMMON_INFO_STARTMODCONF 	21 // начато конфигурирование модуля
#define CODE_COMMON_INFO_SUCMODCONF 	22 // конфигурирование модуля успешно завершено
#define CODE_COMMON_INFO_INTRMODCONFPREV 	23 // конфигурирование модуля прервано с возвратом к предыдущей конфигурации
#define CODE_COMMON_INFO_INTRMODCONFACT 	24 // конфигурирование модуля прервано с использованием актуальной конфигурации
#define CODE_COMMON_INFO_VERSION 	25 // информация о версии ПО модуля
#define CODE_COMMON_INFO_COMMENT 	26 // информация о назначении модуля
#define CODE_COMMON_INFO_CLEAR 	27 // очистка признака аварий модуля
#define CODE_COMMON_INFO_PERIODIC 	28 // периодическая информация о состоянии модуля
#define CODE_COMMON_INFO_STATE 	29 // информация о текущем состоянии модуля
#define CODE_COMMON_INFO_MODTOSERVICEMODE 	31 // модуль переведен в режим обслуживания
#define CODE_COMMON_INFO_MODFROMSERVEICEMODE 	32 // модуль выведен из режима обслуживания
#define CODE_COMMON_INFO_MODFROMSERVICEMODE 	33 // модуль не выведенен из режима обслуживания
#define CODE_COMMON_INFO_OBTTASKTOCTRL 	41 // получено задание ППК по постановке на контроль
#define CODE_COMMON_INFO_OBTTASTFROMCTRL 	42 // получено задание ППК по снятию с контроля
#define CODE_COMMON_INFO_TASKLISTQUERY 	43 // выполнен запрос списка заданий с ППК
#define CODE_COMMON_INFO_TASKLISTELEMOBT 	44 // получен элемент списка заданий с ППК
#define CODE_COMMON_INFO_SUCTASK 	45 // задание ППК успешно выполнено
#define CODE_COMMON_INFO_SUCTASKWITHERR 	46 // задание ППК выполнено с ошибкой
#define CODE_COMMON_INFO_SUCREADTASKLIST 	47 // список заданий с ППК успешно прочитан
#define CODE_COMMON_INFO_INTRTASKLISTBYPPK 	48 // список заданий с ППК прерван по инициативе ППК
#define CODE_COMMON_INFO_INTRTASKLISTBYMOD 	49 // список заданий с ППК прерван по инициативе модуля
#define CODE_COMMON_INFO_CREATENEWSEANS 	50 // создание нового сеанса
#define CODE_COMMON_INFO_OBTPTOCOM 	51 // получена команда ПТО
#define CODE_COMMON_INFO_SUCPTOCOM 	52 // команда ПТО выполнена успешно
#define CODE_COMMON_INFO_PTOCOMERR 	53 // выполнение команды ПТО отвергнуто модулем
#define CODE_COMMON_INFO_PTOCOM 	54 // команда ПТО выполнена с ошибкой
#define CODE_COMMON_INFO_CHANWARN 	61 // предупреждение от канала связи с удаленным устройством
#define CODE_COMMON_INFO_CHANFATAL 	62 // критическая ошибка канала связи с удаленным устройством
#define CODE_COMMON_INFO_EXTDEVMSG 	71 // информационное сообщение от удаленного устройства
#define CODE_COMMON_INFO_EXTDEVWARN 	72 // предупреждение от удаленного устройства
#define CODE_COMMON_INFO_EXTDEVFATAL 	73 // критическая ошибка на удаленном устройстве
#define CODE_COMMON_INFO_SUCFILECOPY 	81 // копирование файла выполнено успешно
#define CODE_COMMON_INFO_SUCDIRCOPY 	82 // копирование директории выполнено успешно
#define CODE_COMMON_INFO_SUCFILEDEL 	83 // файл успешно удален
#define CODE_COMMON_INFO_SUCDIRDEL 	84 // директория успешно удалена
#define CODE_COMMON_INFO_SUCLOGDEL 	85 // все файлы журнала старше указанной даты успешно удалены
#define CODE_COMMON_INFO_SUCFILEMOVE 	86 // перемещение файла выполнено успешно
#define CODE_COMMON_INFO_SUCDIRMOVE 	87 // перемещение директории выполнено успешно
#define CODE_COMMON_INFO_SUCPTINIT 	91 // Инициализация транспортной точки выполнена успешно
#define CODE_COMMON_INFO_SUCPTSTART 	92 // Запуск транпортной точки выполнен успешно
#define CODE_COMMON_INFO_SUCPTSTOP 	93 // Останов транспортной точки выполнен успешно
#define CODE_COMMON_INFO_SUCTHRDPULLINIT 	94 // Инициализация пула потоков выполнена успешно
#define CODE_COMMON_INFO_GOTPKT 	101 // Получен пакет данных
#define CODE_COMMON_INFO_GOTUNKNPKT 	102 // Получен неизвестный пакет данных
#define CODE_COMMON_INFO_BUFFERIZEPKT 	103 // Пакет данных помещен в буфер
#define CODE_COMMON_INFO_GOTBROADCASTNOTIF 	104 // Получено широковещательное сообщение
#define CODE_COMMON_INFO_SENDPKT 	105 // Пакет данных отправлен
#define CODE_COMMON_INFO_RESENDPKT 	106 // Выполнена пересылка пакета данных
#define CODE_COMMON_INFO_FREEPKT 	107 // Освобождение памяти пакета
#define CODE_COMMON_INFO_LAPTO_STATUS 	150 // Информация о работе ЛА ПТО
#define CODE_COMMON_INFO_LAPTO_STATUS_MODULE 	151 // Информация о модуле, контролируемом ЛА ПТО
#define CODE_COMMON_INFO_LAPTO_STATUS_MODULES 	152 // Информация о модулях, контролриуемых ЛА ПТО
#define CODE_COMMON_INFO_RM_LOGIN 	201 // Авторизация в комплексе
#define CODE_COMMON_INFO_RM_LOGOUT 	202 // Выход из сеанса работы
