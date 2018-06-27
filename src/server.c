#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mysql/mysql.h>

#include "server_sock.h"
#include "util.h"
#include "server_sql.h"


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
	char id[10];
	char passwd[10];
	char msg[BUF_SIZE];
	char buf[BUF_SIZE];
	pthread_t t_id[MAX_CLNT] = {0};
	int flag=0;
	MYSQL *conn;
	MYSQL_RES *res_ptr;

	if(argc !=2){
		printf("use : %s <port>\n",argv[0]);
		exit(1);
	}
	fputs("IoT server start!!\n",stdout);
	
	conn =mysql_init(NULL);
	if(sql_connect(conn))
		printf("sql Connection Successful!\n");
	
	serv_sock=gen_serv_sock(argv[1]);
	
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
		
		for(i=0;i<MAX_CLNT;i++){
			if(strcmp(client[i].ip,inet_ntoa(clnt_adr.sin_addr))==0){
				pthread_cancel(client[i].tid);
				reset_clientst(client+i);
				close(client[i].fd);
				clnt_cnt--;
				printf("cancel\n");
				continue;
			}
		}

		memset(buf,0,sizeof(buf));
		str_len=read(clnt_sock,buf,sizeof(buf));
		printf("%s",buf);

		//id password parsing
		parsing(buf,pToken,pArray,"[:]");
		strcpy(id,pArray[0]);
		strcpy(passwd,pArray[1]);

		for(i=0;i<MAX_CLNT;i++){
			if(client[i].fd==0){
				if(check_device(conn,res_ptr,id,passwd)){
					printf("client connect\n");
					client[i].fd=clnt_sock;
					client[i].tid=t_id[i];
					strcpy(client[i].ip ,inet_ntoa(clnt_adr.sin_addr));
					strcpy(client[i].id,id);
					sprintf(msg,"New connected !!(ip:%s, sockcnt%d)\n",client[i].ip , clnt_sock);
					//temp=clnt_cnt++;
					clnt_cnt++;
					printf("%s",msg);
					printf("%d connected \n",clnt_cnt);
					pthread_create(t_id+i,NULL,handle_clnt,(void*)(client+i));

					pthread_detach(t_id[i]);
				}
				else{
					printf("check error!!\n");
					close(clnt_sock);
				}
				break;
			}
		}
	}
	
	mysql_free_result(res_ptr);
	mysql_close(conn);
	close(serv_sock);
	return 0;
}

