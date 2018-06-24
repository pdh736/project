#include <stdio.h>
#include <string.h>
#include "server_sql.h"

static char buff[100];

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

