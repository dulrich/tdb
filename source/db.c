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

#include "config.h" // make sure your config contains the correct values

#include "db.h"

/* ===== MYSQL HELPER FNS ===== */
MYSQL* tea_conn;

int tea_mysql_close() {
	mysql_close(tea_conn);
	tea_conn = NULL;
	return 0;
}

int tea_mysql_error(const char* err_type) {
	if (tea_conn == NULL) return 1;
	
	fprintf(stderr, "mysql %s error:%s\n",err_type,mysql_error(tea_conn));
	return 1;
}

int tea_mysql_query(const char* query) {
	return tea_mysql_query_db(CONFIG.db_name,query);
}

int tea_mysql_query_db(const char* db,const char* query) {
	MYSQL_RES* result = tea_mysql_query_db_res(db,query);
	
	if (result != NULL) mysql_free_result(result);
	
	return 0;
}

MYSQL_RES* tea_mysql_query_res(const char* query) {
	return tea_mysql_query_db_res(CONFIG.db_name,query);
}

MYSQL_RES* tea_mysql_query_db_res(const char* db,const char* query) {
	if (tea_conn == NULL) {
		tea_conn = mysql_init(NULL);
	
		if (tea_conn == NULL) {
			fprintf(stderr, "%s\n", mysql_error(tea_conn));
			return NULL;
		}
		
		if (mysql_real_connect(
			tea_conn,
			CONFIG.db_host,
			CONFIG.db_user,
			CONFIG.db_pass,
			db,
			0,
			NULL,
			CLIENT_MULTI_STATEMENTS
		) == NULL) {
			tea_mysql_error("connection");
			return NULL;
		}
	}
	
	if (mysql_query(tea_conn, query)) {
		tea_mysql_error("query");
		return NULL;
	}
	
	MYSQL_RES* result = mysql_store_result(tea_conn);
	
	// this is only an error if the query should have returned results
	// if (result == NULL) {
	// 	tea_mysql_error("result");
	// }
	
	return result;
}
