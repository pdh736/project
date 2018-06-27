#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 32
#define ID_SIZE 10
#define ARR_CNT 5

#define DEBUG


int clnt_cnt;

typedef struct{
	int index;
	int fd;
	pthread_t tid;
	char ip[20];
	char id[10];
}CLIENT_INFO;

CLIENT_INFO client[MAX_CLNT];

int gen_serv_sock(const char * port);
void *handle_clnt(void *arg);
void reset_clientst(CLIENT_INFO *client);
