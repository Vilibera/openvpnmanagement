#include "includes.h"
#include "uci_alloc.h"

struct uci_context *ubus_ctx = NULL;
struct uci_package *package = NULL;

#define OPT_ABORT  1     /* –abort */
static struct argp_option options[] = {
  {"ipaddress",    'h',"IpAddress", 0,  "Ip Address" },
  {"port",         'p',"Port",      0,  "Port" },
  {"abort",         OPT_ABORT,      0, 0, "Abort before showing any output"},
  { 0 }
};


static error_t parse_opt (int key, char *arg, struct argp_state *state);

const char *argp_program_version ="openvpn management";
const char *argp_program_bug_address ="<viliusbernikas@teltonika.lt>";

static char doc[] ="openvpn management";

static char args_doc[] = "ARG1 [STRING...]";

static struct argp argp = { options, parse_opt, args_doc, doc }; 

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    struct Connection *con = state->input;
    switch (key){
        case 'h': 
        {   
            con->address = arg;
            break;
          
        }
        case 'p':
        {
             con->port = arg;
             break;
            
        }
		case ARGP_KEY_END:
		if ((!con->address) || (!con->port)){
			argp_usage (state);
		}
		break;
		default:
			return ARGP_ERR_UNKNOWN;
 }
 return 0;
} 


void sigHandler(int signo) 
{
	syslog(LOG_INFO,"Program is closing...");
	closelogs(signo);
}

//Main method
int main(int argc,char *argv[])
{
    openlog(NULL, LOG_CONS, LOG_USER);
	struct Connection con;

    con.address=NULL;
    con.port=NULL;

    argp_parse(&argp,argc,argv,0,0,&con);

	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

    uci_alloc(ubus_ctx,CONFIG,&package);

    ubus_init();
    ubus_register_server(package);

    socketfd = socket_init(con.address, con.port);

    uloop_run();
    uloop_done();
    
    CleanAll(0);
    closeSocket(0);
    return 0;

}