#include <mysql/mysql.h>

#define host "localhost"
#define user "root"
#define pass "root"
#define dbname "test"
#define table "clients"


int sql_connect(MYSQL* conn);
int sql_insert(MYSQL *conn,const char * id, const char * passwd);
MYSQL_RES * sql_store_table(MYSQL *conn,const char * query);
void sql_print_all_table(MYSQL *conn, MYSQL_RES *res_ptr);
int check_device(MYSQL *conn, MYSQL_RES *res_ptr,const char * id, const char * passwd);

