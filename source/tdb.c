// tdb: A Silly Program For Tracking Tea Inventory and Consumption
// Copyright (C) 2014 - 2015  David Ulrich
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// 
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"
#include "db.h"

#include <readline/readline.h>
#include <readline/history.h>

#include "config.h" // make sure your config contains the correct values

struct TeaInventory {
	int TeaInventoryID;
	float TeaQuantity;
	char TeaName[128*2+1];
	char TeaType[128*2+1];
	char TeaUnit[128*2+1];
	int TeaServingsPerUnit;
};


/* ===== REGULAR HELPER FNS ===== */
void input_add_inventory();
void input_drink_tea();
int get_line_i(const char* msg);
char* get_line_s(const char* msg);
int int_or_0(char* str);
int list_tea_inventory(int show_nums);
void print_options();


void database_init() {
	char query[128];
	const char* query_db = "CREATE DATABASE IF NOT EXISTS %s";
	char* query_tables;
	
	snprintf(query,sizeof(query),query_db,CONFIG.db_name);
	
	tea_mysql_query_db(NULL,query);
	tea_mysql_close();
	
	query_tables = tea_file_read("/code/tdb/db-setup.sql",NULL);
	
	if (query_tables) {
		tea_mysql_query(query_tables);
		
		free(query_tables);
	}
	
	tea_mysql_close(); // don't leave in a broken state
}

int do_input() {
	char input = getchar();
	
	switch(input) {
		case 'i':
			printf("initializing database.\n");
			database_init();
			printf("done.\n");
			break;
		
		case 'x':
			printf("goodbye.\n");
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
	
	const char* query_template = "UPDATE tea_inventory "
		"SET TeaQuantity = TeaQuantity - %d "
		"WHERE TeaInventoryID = %d";
	
	snprintf(query,sizeof(query),query_template,amount,which);
	
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
	printf(
		"1: view inventory, "
		"2: drink tea, "
		"3: add inventory, "
		"i: initialize db, "
		"x: exit\n"
	);
}


/* ===== MAIN ===== */
int main(/*int argc,char* argv[]*/) {
	char dir[1024];
	char path_config[1024];
	
	getcwd(dir,sizeof(dir));
	
	if (!*dir) {
		printf("unable to get current working directory.\n");
		return 1;
	}
	
	snprintf(path_config,sizeof(path_config),"%s/config.lua",dir);
	
	TDB_config_load(path_config);
	
	print_options();
	while(do_input()) {}
	
	tea_mysql_close();
	
	TDB_config_unload();
	
	return 0;
}
