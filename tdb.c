#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <my_global.h>
#include <mysql.h>

#include <readline/readline.h>
#include <readline/history.h>

static char* TEA_DB_HOST = "localhost";
static char* TEA_DB_USER = "root";
static char* TEA_DB_PASS = "34dS&9ee";
static char* TEA_DB_NAME = "tdb";

struct TeaInventory {
	int TeaInventoryID;
	float TeaQuantity;
	char TeaName[128*2+1];
	char TeaType[128*2+1];
	char TeaUnit[128*2+1];
	int TeaServingsPerUnit;
};


/* ===== MYSQL HELPER FNS ===== */
int tea_mysql_close();
int tea_mysql_error();
int tea_mysql_query(char* query);
MYSQL_RES* tea_mysql_query_res(char* query);

MYSQL* tea_conn;

int tea_mysql_close() {
	mysql_close(tea_conn);
	tea_conn = NULL;
	return 0;
}

int tea_mysql_error() {
	if (tea_conn == NULL) return 1;
	
	fprintf(stderr, "%s\n", mysql_error(tea_conn));
	return 1;
}

int tea_mysql_query(char* query) {
	MYSQL_RES* result = tea_mysql_query_res(query);
	
	if (result != NULL) mysql_free_result(result);
	
	return 0;
}

MYSQL_RES* tea_mysql_query_res(char* query) {
	if (tea_conn == NULL) {
		tea_conn = mysql_init(NULL);
	
		if (tea_conn == NULL) {
			fprintf(stderr, "%s\n", mysql_error(tea_conn));
			return NULL;
		}
		
		if (mysql_real_connect(tea_conn,TEA_DB_HOST,TEA_DB_USER,TEA_DB_PASS,TEA_DB_NAME,0,NULL,0) == NULL) {
			tea_mysql_error(tea_conn);
			return NULL;
		}
	}
	
	if (mysql_query(tea_conn, query)) {
		tea_mysql_error(tea_conn);
		return NULL;
	}
	
	MYSQL_RES* result = mysql_store_result(tea_conn);
	
	if (result == NULL) {
		tea_mysql_error(tea_conn);
	}
	
	return result;
}




/* ===== REGULAR HELPER FNS ===== */
void input_add_inventory();
void input_drink_tea();
int get_line_i(const char* msg);
char* get_line_s(const char* msg);
int int_or_0(char* str);
int list_tea_inventory(int show_nums);
void print_options();


int do_input() {
	char input = getchar();
	
	switch(input) {
		case 'x':
			printf("got x\n");
			return 0;
			break;
			
		case '1':
			printf("listing inventory\n");
			list_tea_inventory(0);
			break;
		
		case '2':
			printf("drinking tea\n");
			list_tea_inventory(1);
			
			input_drink_tea();
			
			break;
		
		case '3':
			printf("add inventory\n");
			list_tea_inventory(1);
			
			input_add_inventory();
			
			break;
			
		default:
			print_options();
	}
	
	return 1;
}


int drink_tea(int which,int amount) {
	char query[1024];
	
	snprintf(query,sizeof(query),"UPDATE tea_inventory SET TeaQuantity = TeaQuantity - %d WHERE TeaInventoryID = %d",amount,which);
	
	return tea_mysql_query(query);
}


int get_line_i(const char* msg) {
	char* line;
	int val;
	
	line = readline(msg);
	
	val = int_or_0(line);
	
	if (line != NULL) free(line);
	
	return val;
}

char* get_line_s(const char* msg) {
	char* line;
	
	line = readline(msg);
	
	return line;
}


int int_or_0(char* str) {
	errno = 0;
	
	long l = strtol(str,NULL,0);
	
	if (errno) return 0;
	
	return (int)l;
}


void input_add_inventory() {
	int amount;
	int tea_id;
	
	tea_id = get_line_i("Which tea needs to be updated? ");
	
	if(tea_id <= 0) return;
	
	amount = -1 * get_line_i("How much should be added? ");
	
	drink_tea(tea_id,amount);
}


void input_drink_tea() {
	int amount;
	int tea_id;
	
	tea_id = get_line_i("Which tea would you like to drink? ");
	
	if(tea_id <= 0) return;
	
	amount = get_line_i("How much did you drink? ");
	
	drink_tea(tea_id,amount);
}


int list_tea_inventory(int show_nums) {
	char* query = "SELECT "
	"	I.TeaInventoryID,I.TeaQuantity,T.TeaName,"
	"	Y.TeaType,U.TeaUnit,I.TeaServingsPerUnit "
	"FROM tea_inventory I "
	"LEFT JOIN tea T ON I.TeaID = T.TeaID "
	"LEFT JOIN tea_type Y ON T.TeaTypeID = Y.TeaTypeID "
	"LEFT JOIN tea_unit U ON I.TeaUnitID = U.TeaUnitID "
	"WHERE I.TeaQuantity > 0 ";
	printf("1\n");
	MYSQL_RES* result = tea_mysql_query_res(query);
	if (result == NULL) return 1;
	
	int num_fields = mysql_num_fields(result);
	int i;
	int row_count = 0;
	MYSQL_ROW row;
	
	while ((row = mysql_fetch_row(result))) 
	{
		row_count++;
		
		if (show_nums) printf("%d) ",row_count);
		
		for(i = 0; i < num_fields; i++) {
			printf("%s ",row[i] ? row[i] : "NULL"); 
		}
		printf("\n"); 
	}
	
	mysql_free_result(result);
	return 0;
}


void print_options() {
	printf("1: view inventory, 2: drink tea, 3: add inventory, x: exit\n");
}


/* ===== MAIN ===== */
int main(/*int argc,char* argv[]*/) {
// 	printf("MySQL client version: %s\n", mysql_get_client_info());
	
	print_options();
	while(do_input()) {}
	
	tea_mysql_close();
	
	return 0;
}
