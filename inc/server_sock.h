#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

#define BUF_SIZE 100
#define MAX_CLNT 32
#define ID_SIZE 10
#define ARR_CNT 5

#define DEBUG


int clnt_cnt;

typedef struct{
	int index;
	int fd;
	char ip[20];
}CLIENT_INFO;


int gen_serv_sock(const char * port);
void *handle_clnt(void *arg);
