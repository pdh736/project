#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server_sock.h"
#include <pthread.h>
#include <mysql/mysql.h>

static char * host = "localhost";
static char * user = "root";
static char * pass = "root";
static char * dbname = "test";



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
	char msg[BUF_SIZE];
	char buf[BUF_SIZE];
	pthread_t t_id[MAX_CLNT] = {0};
	int flag=0;
	CLIENT_INFO client[MAX_CLNT];

	//mysql-------------------------
	MYSQL *conn;
	MYSQL_RES *res_ptr;
	MYSQL_ROW *sqlrow;
	int res;
	conn =mysql_init(NULL);
	if(!(mysql_real_connect(conn,host,user,pass,dbname,0,NULL,0)))
	{
		fprintf(stderr,"ERROR: %s,[%d[\n",mysql_error(conn),mysql_errno(conn));
		exit(1);
	}
	printf("Connection Successful!\n\n");
/*
	//insert into table		
    res = mysql_query(conn, "insert into num(id,password) values (null,'hee')");
	if(!res)
		printf("inserted %1u rows\n",(unsigned long)mysql_affected_rows(conn));
	else{
		fprintf(stderr,"Insert nerror\n");
	}
*/
	/*
	//printf all table
	res = mysql_query(conn,"select * from num");
	if(!res){
		res_ptr=mysql_store_result(conn);
		if(res_ptr){
			printf("Retrived %1u rows\n",(unsigned long)mysql_num_rows(res_ptr));
			while((sqlrow=mysql_fetch_row(res_ptr))){
				printf("%10s %10s \n", sqlrow[0], sqlrow[1]);
			}
		}
		else{
			sprintf(stderr,"store error\n");
		}
	}
	else{
		sprintf(stderr,"query error\n");
	}
	*/
/*
	//check text
	res = mysql_query(conn,"select * from clients where id like \'avr1\'");
	if(!res){
		res_ptr=mysql_store_result(conn);
		if(res_ptr){
			while((sqlrow=mysql_fetch_row(res_ptr))){
				printf("%10s %10s \n", sqlrow[1], sqlrow[2]);
				if(strcmp("avr1",sqlrow[1])==0)printf("correct\n");
			}
		}
		else{
			fprintf(stderr,"store error\n");
		}
	}
	else{
		fprintf(stderr,"query error\n");
	}
	mysql_free_result(res_ptr);
	mysql_close(conn);
*/
	//-------------------------------			

	if(argc !=2){
		printf("use : %s <port>\n",argv[0]);
		exit(1);
	}
	fputs("IoT server start!!\n",stdout);
	
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
		memset(buf,0,sizeof(buf));
		str_len=read(clnt_sock,buf,sizeof(buf));
		printf("%s",buf);

		//db check
		i=0;
		pToken=strtok(buf,"[:]");
		while(pToken !=NULL){
			pArray[i]=pToken;
	//		printf("%d  %s\n",i,pArray[i]);
			if(++i >=ARR_CNT)	break;
			pToken=strtok(NULL,"[:]");
		}
		strcpy(id,pArray[0]);
		sprintf(buf,"select * from clients where id like \'%s\'",id);
		printf("%s\n",buf);
		res = mysql_query(conn,buf);
		if(!res){
			res_ptr=mysql_store_result(conn);
			if(res_ptr){
				while((sqlrow=mysql_fetch_row(res_ptr))){
					printf("%10s %10s \n", sqlrow[1], sqlrow[2]);
					if(strcmp(id,sqlrow[1])==0)printf("correct\n");
				}
			}
			else{
				fprintf(stderr,"store error\n");
			}
		}
		else{
			fprintf(stderr,"query error\n");
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
