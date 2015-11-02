CFLAGS=-I.
CC_zynq=arm-linux-gnueabi-gcc
CC=gcc
LDFLAGS=-pthread

zynq: clean local_app.o_zynq hyper.o_zynq
	$(CC_zynq) $(LDFLAGS) local_app.o hyper.o -o local_app_zynq -lm

pc: clean local_app.o hyper.o
	$(CC) $(LDFLAGS) local_app.o hyper.o -o local_app_pc -lm -ggdb

test_zynq: clean test_distances.o_zynq hyper.o_zynq
	$(CC_zynq) test_distances.o hyper.o -o test_dist_zynq -lm

test: clean test_distances.o hyper.o
	$(CC) test_distances.o hyper.o -o test_dist -lm

test_distances.o_zynq:
	$(CC_zynq) -c test_distances.c

test_distances.o:
	$(CC) -c test_distances.c

local_app.o:
	$(CC) -c local_app.c

hyper.o:
	$(CC) -c hyper.c

local_app.o_zynq:
	$(CC_zynq) -c local_app.c

hyper.o_zynq:
	$(CC_zynq) -c hyper.c

clean:
	rm -f *o
