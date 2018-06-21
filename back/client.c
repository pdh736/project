#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
#define ARR_CNT 5

void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);

char name[NAME_SIZE]="[Default]";   //ID
char msg[BUF_SIZE];

int main(int argc, char * argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	int str_len;

	if(argc !=3){
		printf("use <ip,<port>");
	}
	sock =socket(PF_INET, SOCK_STREAM,0);
	if(sock<0) printf("sock error\n");

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		printf("connect error\n");
	else
		printf("asdfasdf\n");
//	str_len=read(sock,msg,BUF_SIZE);
	sprintf(msg,"ubuntu client\n");
	write(sock,msg,strlen(msg));
	//printf("%s",msg);

}
