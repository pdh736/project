#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mysql/mysql.h>

#include "server_sock.h"
#include "util.h"

static char * host = "localhost";
static char * user = "root";
static char * pass = "root";
static char * dbname = "test";
static char * table1 = "clients";
char buff[100];



int sql_connect(MYSQL* conn)
{
	if(!(mysql_real_connect(conn,host,user,pass,dbname,0,NULL,0)))
	{
		fprintf(stderr,"ERROR: %s,[%d[\n",mysql_error(conn),mysql_errno(conn));
		return 0;
	}
	return 1;
}

int sql_insert(MYSQL *conn,const char * id, const char * passwd)
{
	int res;
	//값 바꿔야 함
	sprintf(buff,"insert into clients values (null,\'%s\',\'%s\')",id,passwd);
	res = mysql_query(conn,buff);
	if(!res){
		printf("inserted %lu rows\n",(unsigned long)mysql_affected_rows(conn));
		return 1;
	}
	else{
		fprintf(stderr,"Insert nerror\n");
		return 0;
	}
}

MYSQL_RES * sql_store_table(MYSQL *conn,const char * query)
{
	int res;
	res=mysql_query(conn,query);
	if(!res){
		return mysql_store_result(conn);
	}
	else{
		printf("result store error!!\n");
		return NULL;
	}
}

void sql_print_all_table(MYSQL *conn, MYSQL_RES *res_ptr)
{
	MYSQL_ROW sqlrow;

	sprintf(buff,"select * from clients");
	if(res_ptr=sql_store_table(conn,buff)){
		while( (sqlrow=mysql_fetch_row(res_ptr)) ){
				printf("%10s %10s \n", sqlrow[1], sqlrow[2]);
		}
	}
	else{
		printf("table read error!!\n");
	}
	
}

int check_device(MYSQL *conn, MYSQL_RES *res_ptr,const char * id, const char * passwd)
{
	MYSQL_ROW sqlrow;

	sprintf(buff,"select * from clients where id like \'%s\'",id);
	if(res_ptr=sql_store_table(conn,buff)){
		while(sqlrow=mysql_fetch_row(res_ptr)){
			if(strcmp(id,sqlrow[1])==0){
				printf("id correct!!\n");
				if(strcmp(passwd,sqlrow[2])==0){
					printf("password correct!!\n");
					return 1;
				}
				else{
					printf("pawssword incorrect!!\n");
					return 0;
				}

			}
			else{
				printf("id incorrect!!!\n");
				return 0;
			}
		}
	}
	else
		printf("table read error!!\n");
	return 0;

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
	char id[10];
	char passwd[10];
	char msg[BUF_SIZE];
	char buf[BUF_SIZE];
	pthread_t t_id[MAX_CLNT] = {0};
	int flag=0;
	CLIENT_INFO client[MAX_CLNT];

	//mysql-------------------------
	MYSQL *conn;
	MYSQL_RES *res_ptr;
	conn =mysql_init(NULL);
	if(sql_connect(conn))
		printf("Connection Successful!\n\n");
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

		//id password check
		parsing(buf,pToken,pArray,"[:]");
		strcpy(id,pArray[0]);
		strcpy(passwd,pArray[1]);
		

		if(check_device(conn,res_ptr,id,"avr1")){
		
			printf("client connect\n");
			client[clnt_cnt].fd=clnt_sock;
			strcpy(client[clnt_cnt].ip ,inet_ntoa(clnt_adr.sin_addr));
			strcpy(client[clnt_cnt].id,id);
			sprintf(msg,"New connected !!(ip:%s, sockcnt%d)\n",client[clnt_cnt].ip , clnt_sock);
			temp=clnt_cnt++;
			printf("%s",msg);
			pthread_create(t_id+temp,NULL,handle_clnt,(void*)(client+temp));

			pthread_detach(t_id[temp]);
			close(clnt_sock);
		}
		else{
			printf("check error!!\n");
		}

	}
	
	mysql_free_result(res_ptr);
	mysql_close(conn);
	return 0;
}
