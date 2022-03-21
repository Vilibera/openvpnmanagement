#include <includes.h>

void closelogs(int rc) 
{
    closelog();
    exit(rc);
   
}

void CleanAll(int rc)
{
    syslog(LOG_INFO,"openvpnmanagement is cleaning up");
    closelogs(rc);

}
void closeSocket(int rc)
{
    close(socketfd);

}