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

#ifndef TDB_DB_H
#define TDB_DB_H

#include <my_global.h>
#include <mysql.h>

int tea_mysql_close();
int tea_mysql_error(const char* err_type);
int tea_mysql_query(const char* query);
int tea_mysql_query_db(const char* db,const char* query);
MYSQL_RES* tea_mysql_query_res(const char* query);
MYSQL_RES* tea_mysql_query_db_res(const char* db,const char* query);

#endif // TDB_DB_H
