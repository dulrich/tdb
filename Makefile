CFLAGS=	-W -Wall -I.. -pthread -g

.PHONY: all clean

all:
	OS=`uname`; \
	  test "$$OS" = Linux && LIBS="-ldl -lreadline" ; \
	  $(CC) $(CFLAGS) tdb.c  $$LIBS $(ADD) -o tdb `mysql_config --cflags --libs`;

clean:
	rm -rf tdb *.exe *.dSYM *.obj
