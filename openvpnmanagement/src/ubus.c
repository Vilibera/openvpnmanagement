#include "includes.h"
#include "uci_alloc.h"
#include "uci_init.h"
#include "socket.h"

#define NAME_MAX 256



enum {
	SERVER_DATA,
	__SERVER_MAX,
};
static struct ubus_context *ubus_ctx = NULL;
static struct blob_buf b;

static const struct blobmsg_policy server_policy[__SERVER_MAX] = {
    [SERVER_DATA] = { .name = "name", .type = BLOBMSG_TYPE_STRING },
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
    int rotations = 0;
    int rc;
	
	blob_buf_init(&b, 0);

    messageptr = socket_message(socketfd, "status");
    rc = strcmp(messageptr,"error");
    tbl = blobmsg_open_array(&b, "Clients");

    ptr = strtok(messageptr, "\n\r");
    while(ptr!=NULL){
        if(rotations<3){
            rotations++;
            ptr = strtok(NULL,"\n\r");
            continue;
        }
        if(strcmp(ptr,"ROUTING TABLE")==0){
            break;
        }
            blobmsg_add_string(&b,NULL,ptr);
        ptr = strtok(NULL,"\n\r");

    }
     blobmsg_close_table(&b,tbl);
     ubus_send_reply(ubus_ctx, req, b.head);
    free(messageptr);
	
	return 0;
}


 static const struct ubus_method server_methods[] = {
    UBUS_METHOD_NOARG("status", server_status),
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

