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

#include <my_global.h>
#include <mysql.h>

#include "../config.h" // make sure your config contains the correct values

#include "db.h"

/* ===== MYSQL HELPER FNS ===== */
MYSQL* tea_conn;

int tea_mysql_close() {
	mysql_close(tea_conn);
	tea_conn = NULL;
	return 0;
}

int tea_mysql_error() {
	if (tea_conn == NULL) return 1;
	
	fprintf(stderr, "mysql error:%s\n", mysql_error(tea_conn));
	return 1;
}

int tea_mysql_query(char* query) {
	return tea_mysql_query_db(TEA_DB_NAME,query);
}

int tea_mysql_query_db(char* db,char* query) {
	MYSQL_RES* result = tea_mysql_query_db_res(db,query);
	
	if (result != NULL) mysql_free_result(result);
	
	return 0;
}

MYSQL_RES* tea_mysql_query_res(char* query) {
	return tea_mysql_query_db_res(TEA_DB_NAME,query);
}

MYSQL_RES* tea_mysql_query_db_res(char* db,char* query) {
	if (tea_conn == NULL) {
		tea_conn = mysql_init(NULL);
	
		if (tea_conn == NULL) {
			fprintf(stderr, "%s\n", mysql_error(tea_conn));
			return NULL;
		}
		
		if (mysql_real_connect(tea_conn,TEA_DB_HOST,TEA_DB_USER,TEA_DB_PASS,db,0,NULL,0) == NULL) {
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
