CFLAGS=	-W -Wall -I.. -pthread -g

all:
	OS=`uname`; \
	  test "$$OS" = Linux && LIBS="-ldl" ; \
	  $(CC) $(CFLAGS) tdb.c  $$LIBS $(ADD) -o tdb `mysql_config --cflags --libs`;

clean:
	rm -rf tdb *.exe *.dSYM *.obj