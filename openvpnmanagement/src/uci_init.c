#include "includes.h"
#include "uci_alloc.h"


void uci_element_check(struct uci_option *option, char* type, char** value)
{
	if(strcmp(option->e.name, type) == 0)
		*value=option->v.string;
}

int uci_element_parse_checks(struct uci_package *package, char **ServerName)
{

	struct uci_element *i, *j;
    char *port = NULL;
	uci_foreach_element(&package->sections, i){
		struct uci_section *section = uci_to_section(i);
		char *section_type = section->type;
        char *section_name = section->e.name;
		if(strcmp(section_type,"openvpn") == 0){
			uci_foreach_element(&section->options, j){
				
				struct uci_option *option = uci_to_option(j);
				uci_element_check(option,"_name", ServerName);
			    uci_element_check(option,"port", &port);
			}
			// printf("port: %s \n",port);
			// printf("CON port: %s \n", con.port);
			// if(strcmp(port, con.port)==0){
			 	return 0;
			// 	printf("12 \n");
			// }
				
		}
	}
    return -1;
}