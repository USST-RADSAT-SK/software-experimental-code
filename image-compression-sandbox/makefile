CC      = gcc
CFLAGS  = -Wall -Wextra
RM      = rm -f


default: all

all: Compress

Compress: compression.o lzfx.o lz4.o
	$(CC) -o compress compression.o lzfx.o lz4.o

compression.o: src/compression.c inc/compression.h
		$(CC) $(CFLAGS) -c src/compression.c

lzfx.o: src/lzfx.c inc/lzfx.h
	$(CC) $(CFLAGS) -c src/lzfx.c

lz4.o: src/lz4.c inc/lz4.h
	$(CC) $(CFLAGS) -c src/lz4.c

.PHONY: clean

clean:
	rm *.o compress
