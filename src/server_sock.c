#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server_sock.h"
#include "util.h"

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
	CLIENT_INFO *t_client=(CLIENT_INFO*)arg;
	int str_len=0;
	char msg[BUF_SIZE];
	int i;
	char *pToken;
	char *pArray[ARR_CNT]={0};
	char strBuff[BUF_SIZE]={0};
	int flag=0;

	printf("client handle\n");
	
	str_len=read(t_client->fd,msg,sizeof(msg));
	printf("%s",msg);
	//sprintf(msg,"[avr1:AIR@ON]\r\n");
//	write(client->fd,msg,strlen(msg));
//	printf("%s",msg);
	sprintf(msg,"[avr1:LED@OFF]\r\n");
	printf("%s",msg);
	write(client->fd,msg,strlen(msg));


	while(1){
	/*
		memset(msg,0,sizeof(msg));
		str_len=read(t_client->fd,msg,sizeof(msg));
		
	//	write(client->fd,msg,strlen(msg));
		
		if(str_len <= 0){
			break;
		}
		//msg[str_len]='\n';
		
		printf("%s",msg);
		parsing(msg,pToken,pArray,"[:@]");
		printf("%s %s\n",pArray[1],pArray[2]);

		if(flag==0){
			printf("send msg\n");
			sprintf(msg,"[avr]air#on\r\n");
			write(client->fd,msg,strlen(msg));
			flag=1;
		}
	
		if(strcmp(pArray[1],"POWER")==0){
			if(strcmp(pArray[2],"GET")==0){
				sprintf(msg,"[server:POWER@400]");
				for(i=0;i<MAX_CLNT;i++){
					if(strcmp(client[i].id,"avr2")==0){
						write(client[i].fd,msg,strlen(msg));
					}
				}
			}
		}
		*/
	}
	printf("close socket\n");
	close(t_client->fd);
	reset_clientst(t_client);
	clnt_cnt--;
	return 0;
}

void reset_clientst(CLIENT_INFO *t_client)
{
	t_client->fd=0;
	t_client->tid=0;
	memset(t_client->ip,'0',sizeof(client->ip));
	memset(t_client->id,'0',sizeof(client->id));
}

