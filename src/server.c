#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "server_sock.h"
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
	char ip[20];
}CLIENT_INFO;


void *handle_clnt(void *arg)
{
	CLIENT_INFO *client=(CLIENT_INFO*)arg;
	int str_len=0;
	char msg[BUF_SIZE];
	int i;
	char *pToken;
	char *pArray[ARR_CNT]={0};
	char strBuff[BUF_SIZE]={0};


	printf("client handle");
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
	
	}
	close(client->fd);
	clnt_cnt--;


}



int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	int sock_option =1;
	int str_len =0;
	int temp;
	int i;
	char *pToken;
	char *pArray[ARR_CNT];
	char msg[BUF_SIZE];
	char buf[BUF_SIZE];
	pthread_t t_id[MAX_CLNT] = {0};
	int flag=0;
	CLIENT_INFO client[MAX_CLNT];

	if(argc !=2){
		printf("use : %s <port>\n",argv[0]);
		exit(1);
	}
	fputs("IoT server start!!\n",stdout);
	
	serv_sock=gen_serv_sock(argv[1]);
	
/*	
	printf("accept.....\n");
	clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
	if(clnt_sock<0)
		printf("error");
	else
	printf("connected\n");
	
	str_len=read(clnt_sock,buf,sizeof(buf));
	printf("%s",buf);

	sprintf(msg ,"[avr]led#on\r\n");

	printf("%s",msg);
*/
	
	while(1){
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock<=0){
			printf("fail to accept\n");
			continue;
		}
		if(clnt_cnt==MAX_CLNT){
			printf("socket full\n");
			shutdown(clnt_sock , SHUT_WR);
			continue;
		}
		printf("client connect\n");
		client[clnt_cnt].fd=clnt_sock;
		strcpy(client[clnt_cnt].ip ,inet_ntoa(clnt_adr.sin_addr)); 
		sprintf(msg,"New connected !!(ip:%s, sockcnt%d)\n",client[clnt_cnt].ip , clnt_sock);
		temp=clnt_cnt++;
		printf("%s",msg);
		pthread_create(t_id+temp,NULL,handle_clnt,(void*)(client+temp));

		pthread_detach(t_id[temp]);

	}
}
