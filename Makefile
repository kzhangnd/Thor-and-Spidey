CC=		gcc
CFLAGS=		-g -Wall -Werror -std=gnu99 -Iinclude
LD=		gcc
LDFLAGS=	-L.
AR=		ar
ARFLAGS=	rcs
TARGETS=	bin/spidey

all:		$(TARGETS)

bin/spidey:		src/spidey.o lib/libspidey.a
	@echo Linking $@...
	@$(LD) $(LDFLAGS) -o $@ $^

lib/libspidey.a:	src/forking.o src/handler.o src/request.o src/single.o src/socket.o src/utils.o
	@echo Linking $@...
	@$(AR) $(ARFLAGS) $@ $^

%.o:			%.c
	@echo Compiling $@...
	@$(CC) -c $(CFLAGS) -o $@ $^


clean:
	@echo Cleaning...
	@rm -f $(TARGETS) lib/*.a src/*.o *.log *.input

.PHONY:		all test clean
