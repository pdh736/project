#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server_sock.h"


int gen_serv_sock(const char * port){
	int serv_sock;
	struct sockaddr_in serv_adr;
	int sock_option=1;

	serv_sock = socket(PF_INET, SOCK_STREAM,0);
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(port));

	setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,(void*)&sock_option,sizeof(sock_option));

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		printf("error");

	if(listen(serv_sock,5)==-1)
		printf("listen error");

	return serv_sock;
}


void *handle_clnt(void *arg)
{
	CLIENT_INFO *client=(CLIENT_INFO*)arg;
	int str_len=0;
	char msg[BUF_SIZE];
	int i;
	char *pToken;
	char *pArray[ARR_CNT]={0};
	char strBuff[BUF_SIZE]={0};
	int flag=0;

	printf("client handle\n");

	while(1){
		memset(msg,0,sizeof(msg));
		str_len=read(client->fd,msg,sizeof(msg));
		if(str_len<=0)
			break;

		msg[str_len]='\n';

		pToken=strtok(msg,"[:]");
		i=0;
		while(pToken !=NULL){
			printf("%s",pToken);
			pArray[i]=pToken;
			if(++i>=ARR_CNT)
				break;
			pToken=strtok(NULL,"[:]");

		}
		if(flag==0){
			printf("send msg\n");
			sprintf(msg,"[avr]air#on\r\n");
			write(client->fd,msg,strlen(msg));
			flag=1;
		}

	}
	close(client->fd);
	clnt_cnt--;


}

