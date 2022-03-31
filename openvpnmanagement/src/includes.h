#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <uci.h>
#include <argp.h>
#include <curl/curl.h>
#include "logscleanup.h"
#include "StructAddress.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <libubus.h>
#include <libubox/uloop.h>
#include <libubox/blobmsg_json.h>
#include <poll.h>
#include <fcntl.h>




#define CONFIG "openvpn"
struct  Connection con;


int socketfd;