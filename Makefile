SRC_DIR = source
OBJ_DIR = object
LIBS = #lua5.2 dl readline

CC		= gcc
DEBUG		= -g
CFLAGS		= -W -Wall -I.. -pthread -g 
LIBCFLAGS	= $(shell pkg-config --cflags $(LIBS) )
LIBLFLAGS	= $(shell pkg-config --libs $(LIBS) )
LFLAGS		= -L./$(OBJ_DIR) -ldl -lreadline
MYSQLFLAGS	= `mysql_config --cflags --libs`
LINK		= $(CC)

TARGET = tdb
EXT = c
SRCS = $(shell find $(SRC_DIR) -type f -name '*.$(EXT)')
OBJS = $(SRCS:$(SRC_DIR)/%.$(EXT)=$(OBJ_DIR)/%.o)

.PHONY: all clean
.SUFFIXES: .o .c

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXT)
	$(CC) $(CFLAGS) $(LIBCFLAGS) $(MYSQLFLAGS) $(DEBUG) -o $@ -c $<

all: $(TARGET)
	$(LINK) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBLFLAGS) $(MYSQLFLAGS)

$(TARGET): $(OBJS)

clean:
	rm -f $(shell find $(OBJ_DIR)/ -type f -name '*.o') 2> /dev/null
	rm -f $(TARGET) 2> /dev/null
