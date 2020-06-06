CC      = gcc
CFLAGS  = -Wall -Wextra
RM      = rm -f


default: all

all: Compress

Compress: compression.o lzfx.o lzf_c.o lzf_d.o
	$(CC) -o compress compression.o lzfx.o lzf_c.o lzf_d.o

compression.o: src/compression.c inc/compression.h
		$(CC) $(CFLAGS) -c src/compression.c

lzfx.o: src/lzfx.c inc/lzfx.h
	$(CC) $(CFLAGS) -c src/lzfx.c

lzf_c.o: src/lzf_c.c inc/lzfP.h
	$(CC) -c src/lzf_c.c

lzf_d.o: src/lzf_d.c inc/lzfP.h
	$(CC) -c src/lzf_d.c

.PHONY: clean

clean:
	rm *.o compress
