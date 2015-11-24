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

#include "config.h"

/* ===== Lua Includes ===== */
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


typedef enum {
	TDB_CONFIG_DOUBLE,
	TDB_CONFIG_INT,
	TDB_CONFIG_FLOAT,
	TDB_CONFIG_STRING
} TDB_config_type;

TDB_config CONFIG = {
	.db_host = NULL,
	.db_user = NULL,
	.db_pass = NULL,
	.db_name = NULL
};

typedef struct {
	const char* name;
	const TDB_config_type type;
	const void* pointer;
} TDB_config_option;

static const TDB_config_option options[] = {
	{ "DB_HOST", TDB_CONFIG_STRING, (void*)&CONFIG.db_host },
	{ "DB_USER", TDB_CONFIG_STRING, (void*)&CONFIG.db_user },
	{ "DB_PASS", TDB_CONFIG_STRING, (void*)&CONFIG.db_pass },
	{ "DB_NAME", TDB_CONFIG_STRING, (void*)&CONFIG.db_name }
};
int TDB_option_count = sizeof(options) / sizeof(TDB_config_option);

void TDB_config_load(char* path) {
	int i;
	
	lua_State* L;
	L = luaL_newstate();
	luaL_openlibs(L);
	
	if (luaL_loadfile(L,path) || lua_pcall(L,0,0,0)) {
		printf("error in config.lua: %s",lua_tostring(L,-1));
	}
	
	for(i=0;i<TDB_option_count;i++) {
		lua_getglobal(L,options[i].name);
		
		if (options[i].type == TDB_CONFIG_DOUBLE) {
			if (lua_isnumber(L,-1)) {
				*(double*)options[i].pointer = (double)lua_tonumber(L,-1);
			}
			else {
				printf(
					"dounble config option %s was not a number:%s\n",
					options[i].name,
					lua_tostring(L,-2)
				);
			}
		}
		else if (options[i].type == TDB_CONFIG_INT) {
			if (lua_isnumber(L,-1)) {
				*(int*)options[i].pointer = (int)lua_tonumber(L,-1);
			}
			else {
				printf(
					"int config option %s was not a number:%s\n",
					options[i].name,
					lua_tostring(L,-2)
				);
			}
		}
		else if (options[i].type == TDB_CONFIG_FLOAT) {
			if (lua_isnumber(L,-1)) {
				*(float*)options[i].pointer = (float)lua_tonumber(L,-1);
			}
			else {
				printf(
					"float config option %s was not a number:%s\n",
					options[i].name,
					lua_tostring(L,-2)
				);
			}
		}
		else if (options[i].type == TDB_CONFIG_STRING) {
			*(char**)options[i].pointer = strdup(lua_tostring(L,-1));
		}
	}
	
	lua_close(L);
}

void TDB_config_unload() {
	int i;
	
	for(i=0;i<TDB_option_count;i++) {
		if (options[i].type == TDB_CONFIG_STRING) {
			free(*(char**)options[i].pointer);
		}
	}
}
