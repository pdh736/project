#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
//#include <fcntl.h>
#include <dirent.h>


int gen_serv_sock(const char * port);
