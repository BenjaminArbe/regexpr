CC=gcc
AR=ar
CFLAGS=-Wall -Wextra -I. -L. -DNDEBUG
# Use make -e DBG=x
ifdef DBG
	CFLAGS=-Wall -Wextra -I. -L. -DNDEBUG -g
endif
SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
TSTSRC=$(wildcard test/*_t.c)
TST=$(patsubst %.c,%,$(TSTSRC))
LIB:=libgen.a
LDLIBS=

INSTALL_DIR=/usr/local

all:	$(LIB) test

$(LIB): $(OBJ)
	$(AR) r $@ $(OBJ) 

.PHONY: test 
test:  LDLIBS += -lgen
test:	$(TST)
	sh test/runtest.sh

valgrind:
	#VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)
	rm -rf /tmp/valgrind-*
	VALGRIND=`valgrind --leak-check=full --log-file=/tmp/valgrind-%p.log $(MAKE)`

clean:
	rm -rf *.a *.o
	rm -f test/*_t test/test.log

install: $(LIB)
	install -m 644 -o root regexpr.h $(INSTALL_DIR)/include
	install -m 755 -o root libgen.a $(INSTALL_DIR)/lib
	
	


