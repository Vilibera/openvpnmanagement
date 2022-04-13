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


static const struct blobmsg_policy server_policy[] = {
    [SERVER_DATA] = {.name = "name", .type = BLOBMSG_TYPE_STRING},
 };

 static void ubus_response_error();


static int server_status(struct ubus_context *ubus_ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{


    void *tbl;
	struct blob_buf b = {};
    char *messageptr = NULL;
    char *ptr = NULL;
    int rot = 0;
    int rc;
    int ind = 0;
    char *payload;
    int ip = 1255;

	char *buff_name = calloc(26, sizeof(char));
    char *buff_realip = calloc(23, sizeof(char));
    char *buff_virtip = calloc(16, sizeof(char));
   	char *buff_lastref = calloc(20, sizeof(char));
    char *buff_cid = calloc(6, sizeof(char));


    // ind = parcel(buff_name, payload, ind);
	// ind = parcel(buff_realip, payload, ind);
	// ind = parcel(buff_virtip, payload, ind) + 1;

	blob_buf_init(&b, 0);
    messageptr = socket_message(socketfd, "status");
    rc = strcmp(messageptr,"error");
    tbl = blobmsg_open_array(&b, "Clients");
    ptr = strtok(messageptr,"\n\r");
    // while(ptr!=NULL){
    //     if(rot<3){
    //         rot++;
    //         ptr = strtok(NULL,"\n\r");
    //         continue;
    //     }
    //     if(strcmp(ptr,"ROUTING TABLE")==0){
    //         ptr = strtok(NULL,",");
    //         blobmsg_add_string(&b,NULL,ptr);
    //     }
    //     // while(strcmp(ptr,"ROUTING TABLE")!=0){
    //     //     ptr=strtok(NULL,",");
    //     //     if(strcmp(ptr,"ROUTING TABLE")==0){
    //     //         break;
    //     //     }
    //     // }
       




    //        // blobmsg_add_string(&b,NULL,ptr);
    //         ptr = strtok(NULL,"\n\r");

    // }
     while(ptr!=NULL){
        if(rot<3){
            rot++;
            ptr = strtok(NULL,"\n\r");
            continue;
        }
        if(strcmp(ptr,"ROUTING TABLE")==0){
            break;
        }
            blobmsg_add_string(&b,NULL,ptr);
        ptr = strtok(NULL,"\n\r");

    }
            blobmsg_add_string(&b, "client name", "CLient1");
		    blobmsg_add_string(&b, "CID", "id");
		    blobmsg_add_u32(&b, "virtual address", ip);
            blobmsg_add_string(&b, "real address", "real");
            blobmsg_add_string(&b, "last reference", "date");


	// for (int i = 0; i < 2; i++) {
    //          while (payload[ind++] != ',') {;}
    // 	}
    // 	ind = parcel(buff_lastref, payload, ind);
    // 	for (int i = 0; i < 2; i++) {
    //          while (payload[ind++] != ',') {;}
    // 	}
   
    // 	ind = parcel(buff_cid, payload, ind);
    ptr = strtok(NULL,"\n\r");
    // print_clients(&b);
    blobmsg_close_table(&b,tbl);
    ubus_send_reply(ubus_ctx, req, b.head);
    free(messageptr);

	return 0;
}



 static const struct ubus_method server_methods[] = {
    UBUS_METHOD_NOARG("clients", server_status),
 };

 static void event_handler_cb(struct uloop_timeout *timeout)
{
    read_openvpn_clients();

    uloop_timeout_set(timeout, 6000);
}

static struct uloop_timeout event_timer = {
    .cb = event_handler_cb,
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

