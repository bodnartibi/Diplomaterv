CFLAGS=-I.
CC_zynq=arm-linux-gnueabi-gcc
CC=gcc
LDFLAGS=-pthread

zynq: clean local_app.o_zynq hyper.o_zynq list.o_zynq worker.o_zynq
	$(CC_zynq) $(LDFLAGS) local_app.o hyper.o list.o worker.o -o local_app_zynq -lm

pc: clean local_app.o hyper.o list.o worker.o
	$(CC) $(LDFLAGS) local_app.o hyper.o list.o worker.o -o local_app_pc -lm -ggdb

tests: test_dist

test_dist: test_dist_zynq test_dist_pc

test_dist_zynq: clean test_distances.o_zynq 
	cd tests && $(CC_zynq) test_distances.o -o test_dist_zynq -lm

test_dist_pc: clean test_distances.o 
	cd tests && $(CC) test_distances.o -o test_dist -lm

test_distances.o_zynq:
	cd tests && $(CC_zynq) -c test_distances.c -I../

test_distances.o:
	cd tests && $(CC) -c test_distances.c -I../

local_app.o:
	$(CC) -c local_app.c

hyper.o:
	$(CC) -c hyper.c

list.o:
	$(CC) -c list.c

worker.o:
	$(CC) -c worker.c

local_app.o_zynq:
	$(CC_zynq) -c local_app.c

hyper.o_zynq:
	$(CC_zynq) -c hyper.c

list.o_zynq:
	$(CC_zynq) -c list.c

worker.o_zynq:
	$(CC_zynq) -c worker.c

clean:
	rm -f *o
