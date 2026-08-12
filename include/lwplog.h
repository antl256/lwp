#ifndef LWPLOG_H
#define LWPLOG_H

#include "lwpdef.h"

#include <stdio.h>

#define lwp_log_info_overwrite(format, ...) fprintf(stdout, "\033[1G\033[1K" LWP_NAME ": \e[34minfo:\e[0m " format, ##__VA_ARGS__)
#define lwp_log_info(format, ...)           fprintf(stdout, LWP_NAME ": \e[34minfo:\e[0m " format "\n", ##__VA_ARGS__)
#define lwp_log_warning(format, ...)        fprintf(stderr, LWP_NAME ": \033[33mwarning:\033[0m " format "\n", ##__VA_ARGS__)
#define lwp_log_error(format, ...)          fprintf(stderr, LWP_NAME ": \033[31merror:\033[0m " format "\n", ##__VA_ARGS__)

#endif /* LWPLOG_H */