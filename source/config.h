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

#ifndef TDB_CONFIG_H
#define TDB_CONFIG_H

typedef struct {
	char* db_host;
	char* db_user;
	char* db_pass;
	char* db_name;
} TDB_config;

TDB_config CONFIG;

void TDB_config_load(char* path);
void TDB_config_unload();

#endif // TDB_CONFIG_H
