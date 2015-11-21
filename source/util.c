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

#include "util.h"

/* ===== UTILITY FNS ===== */
char* tea_file_read(char* path,int* len) {
	int size;
	char* out;
	FILE* file;
	
	file = fopen(path, "rb");
	
	if(!file) {
		fprintf(stderr, "file read error: %s\n",path);
		return NULL;
	}
	
	fseek(file,0,SEEK_END);
	size = ftell(file);
	rewind(file);
	
	out = (char*)malloc(size + 1);
	
	fread(out,sizeof(char),size,file);
	out[size] = 0;
	
	fclose(file);
	
	if(len) *len = size;
	
	return out;
}
