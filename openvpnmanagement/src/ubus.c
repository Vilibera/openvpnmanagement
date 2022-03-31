#include "includes.h"
#include "uci_alloc.h"
#include "uci_init.h"
#include "socket.h"

#define NAME_MAX 256




static struct ubus_context *ubus_ctx = NULL;
static struct blob_buf b;
static char *type;
static uint32_t client;
static uint32_t tx;
static uint32_t rx;
static char *since;

enum {
	SERVER_DATA,
	SERVER_MAX,
    SERVER_TYPE,
    SERVER_CLIENT,
    SERVER_TX,
    SERVER_RX,
    SERVER_SINCE,
};
//Test
// #define UBUS_LISTEN_OBJ_STR	"client1"
// static struct ubus_event_handler listener;

// static void receive_event(struct ubus_context *ctx, struct ubus_event_handler *ev,
// 			  const char *type, struct blob_attr *msg)
// {
// 	char *str;

// 	str = blobmsg_format_json(msg, true);
// 	printf("{ \"%s\": %s }\n", type, str);
// 	free(str);
// }

// static void listen_main(void)
// {
// 	const char *event_listen = NULL;
// 	int ret = 0;

// 	memset(&listener, 0, sizeof(listener));
// 	listener.cb = receive_event;
// 	event_listen = UBUS_LISTEN_OBJ_STR;
// 	ret = ubus_register_event_handler(ubus_ctx, &listener, event_listen);
// 	if (ret)
// 	{
// 		fprintf(stderr, "Error while registering for event '%s': %s\n",
// 			event_listen, ubus_strerror(ret));
// 		return;
// 	}
	
// 	uloop_init();
// 	ubus_add_uloop(ubus_ctx);
// 	uloop_run();
// 	uloop_done();
// }



//


static const struct blobmsg_policy server_policy[] = {
    [SERVER_DATA] = {.name = "name", .type = BLOBMSG_TYPE_STRING},
    //[SERVER_TYPE] = {.name = "type", .type = BLOBMSG_TYPE_STRING},
    // [SERVER_CLIENT] = {.name = "client_ip", .type = BLOBMSG_TYPE_INT32 },
    // [SERVER_RX] = {.name = "rx_data", .type = BLOBMSG_TYPE_INT32 },
    // [SERVER_TX] = {.name = "tx_data", .type = BLOBMSG_TYPE_INT32 },
   // [SERVER_SINCE] = { .name = "since", .type = BLOBMSG_TYPE_STRING },
 };

 static void ubus_response_error();
 static int server_status(struct ubus_context *ubus_ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
    struct blob_attr *tb[SERVER_MAX];
    int count;
    void *tbl;
	struct blob_buf b = {};
    char *messageptr = NULL;
    char *ptr = NULL;
    char *typetest = NULL;
    char *token;
    int rott = 0;
    int rottt = 0;
    int rot = 0;
    int rotr = 0;
    char message[1024];
    int rc;

    // blobmsg_parse(server_policy, SERVER_MAX, tb, blob_data(msg), blob_len(msg));
    // memset(type, '\0', sizeof(type));
    // strcpy(type,blobmsg_get_string(tb[SERVER_TYPE]));
    // client = blobmsg_get_u32(tb[SERVER_CLIENT]);
    // rx = blobmsg_get_u32(tb[SERVER_RX]);
    // tx = blobmsg_get_u32(tb[SERVER_TX]);
    // since = blobmsg_get_string(tb[SERVER_SINCE]);
	
	blob_buf_init(&b, 0);
    
    messageptr = socket_message(socketfd, "status");
    rc = strcmp(messageptr,"error");

    tbl = blobmsg_open_array(&b, "Clients");
    // ptr = strtok(messageptr, "\r \n ,");
     ptr = strtok(messageptr, "\n \r ,");
    //sprintf(message, "Subject: Received message from: \r\nTopic value is == \r\nMessage value is == \r\n key: \r\n Message value  Topic value");



    while(ptr!=NULL){
        if(rot<=3){
           rot++;
            ptr = strtok(NULL,"\n");
            // printf("%s\n", ptr);
            // blobmsg_add_string(&b,NULL,ptr);
            continue;
        }
    
        if(rott<=11){
            if(rott==0){
                for(int i=0;i<1;i++){
                    printf("\n");
                }
            }
            if(rott==6){
                for(int i=0;i<1;i++){
                printf("\n");

                }  
            }
            if(rott==12){
                for(int i=0;i<1;i++){
                printf("\n");

                } 
            }
            ptr = strtok(NULL,"\n , ");
            printf("%s\n",ptr);
            blobmsg_add_string(&b,NULL,ptr);
            rott++;
            continue;

        }
        if(rottt<=1){
            if(rottt==0){
                for(int i=0;i<1;i++){
                    printf("\n");
                }
            }
            if(rottt==1){
                for(int i=0;i<1;i++){
                    printf("\n");
                }
            }
            ptr = strtok(NULL,"\n");
           printf("%s\n", ptr);
           blobmsg_add_string(&b,NULL,ptr);
            rottt++;
            continue;
        }
        printf("\n");
        if(rotr<=12){
            if(rotr==5){
                for(int i=0;i<1;i++){
                printf("\n");

                }  
            }
            ptr = strtok(NULL,"\n , ");
            printf("%s",ptr);
            blobmsg_add_string(&b,NULL,ptr);
            rotr++;
            break;
        }
            // printf("%s\n", ptr);
            // blobmsg_add_string(&b,NULL,ptr);
            ptr = strtok(NULL,"\n\r");





// while(ptr!=NULL){
//         if(rotations<3){
//             rotations++;
//             ptr = strtok(NULL,"\n\r");
//             continue;
//         }
//         if(strcmp(ptr,"ROUTING TABLE")==0){
//             break;
//         }
//             blobmsg_add_string(&b,NULL,ptr);
//         ptr = strtok(NULL,"\n\r");

    

           
            // printf("%s\n",ptr);
            // // blobmsg_add_string(&b,NULL,ptr);
            // blobmsg_add_string(&b,NULL,ptr);
           // ptr = strtok(NULL,"\r \n ");

    }
    blobmsg_close_table(&b,tbl);
    ubus_send_reply(ubus_ctx, req, b.head);
    free(messageptr);
	
	return 0;

}

 static const struct ubus_method server_methods[] = {
    UBUS_METHOD_NOARG("clients", server_status),
 };

struct ubus_object_type server_object_type = 
    UBUS_OBJECT_TYPE("Server",server_methods);


static struct ubus_object server_object = {
    .name="",
    .type = &server_object_type,
    .methods = server_methods,
    .n_methods = ARRAY_SIZE(server_methods),
};

void ubus_register_server(struct uci_package *package)
{
    int rc;
    char *ServerName =NULL;
    char objectname[NAME_MAX];

    rc = uci_element_parse_checks(package,&ServerName);
    if((rc>0)){
        syslog(LOG_ERR,"Failed to parse server name from uci");
        CleanAll(1);
    }

    snprintf(objectname,NAME_MAX,"openvpn.%s",ServerName);
    server_object.name = objectname;
    rc = ubus_add_object(ubus_ctx,&server_object);
    if(rc){
        syslog(LOG_ERR,"Failed to add server object: %s",ubus_strerror(rc));
        CleanAll(1);
    }
    ubus_add_uloop(ubus_ctx);
}

void ubus_init(void)
{
    uloop_init();
    ubus_ctx = ubus_connect(NULL);
	if (!ubus_ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		CleanAll(1);
	}
    // listen_main();
}

void ubus_finish(void)
{

    if(ubus_ctx == NULL){
        return;
    }
    ubus_remove_object(ubus_ctx, &server_object);
    syslog(LOG_INFO,"Ubus object removed");
    ubus_free(ubus_ctx);
    ubus_ctx ==NULL;

}

