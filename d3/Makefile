CC=gcc
CFLAGS=-g -Wall
COMS=../common
SRCS= $(wildcard $(CURDIR)/*.c $(COMS)/*.c)
OBJS=main.o $(COMS)/common.o
BIN=main 
VERBOSE = FALSE

all:$(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test:
	echo start  `date +%s` > test.log
	for number in `seq 1 1000` ; do \
		./main input ; \
	done
	echo finish `date +%s` >> test.log 
	cat test.log

clean:
	rm -rv $(BIN) *.o $(COMS)/*.o *.log
