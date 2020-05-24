CC      = gcc
CFLAGS  = -Wall -Wextra
RM      = rm -f


default: all

all: Compress

Compress: compression.o lzfx.o
	$(CC) -o compress compression.o lzfx.o

compression.o: src/compression.c inc/compression.h
		$(CC) $(CFLAGS) -c src/compression.c

lzfx.o: src/lzfx.c inc/lzfx.h
	$(CC) $(CFLAGS) -c src/lzfx.c

.PHONY: clean

clean:
	rm *.o compress
