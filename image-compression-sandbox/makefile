CC      = gcc
CFLAGS  = -Wall -Wextra
RM      = rm -f


default: all

all: Compress


Compress: compression.o lzfx.o lz4.o compress.o deflate.o crc32.o adler32.o trees.o zutil.o lzf_c.o lzf_d.o
	$(CC) -o compress compression.o lzfx.o lz4.o compress.o deflate.o crc32.o adler32.o trees.o zutil.o lzf_c.o lzf_d.o

compression.o: src/compression.c inc/compression.h
		$(CC) $(CFLAGS) -c src/compression.c

lzfx.o: src/lzfx.c inc/lzfx.h
	$(CC) $(CFLAGS) -c src/lzfx.c


lz4.o: src/lz4.c inc/lz4.h
	$(CC) $(CFLAGS) -c src/lz4.c

adler32.o: src/adler32.c
	$(CC) $(CFLAGS) -c src/adler32.c

crc32.o: src/crc32.c inc/crc32.h
	$(CC) $(CFLAGS) -c src/crc32.c

trees.o: src/trees.c inc/trees.h
	$(CC) $(CFLAGS) -c src/trees.c

zutil.o: src/zutil.c inc/zutil.h
	$(CC) $(CFLAGS) -c src/zutil.c

deflate.o: src/deflate.c inc/deflate.h
	$(CC) $(CFLAGS) -c src/deflate.c

compress.o: src/compress.c
	$(CC) $(CFLAGS) -c src/compress.c

lzf_c.o: src/lzf_c.c inc/lzfP.h
	$(CC) -c src/lzf_c.c

lzf_d.o: src/lzf_d.c inc/lzfP.h
	$(CC) -c src/lzf_d.c


.PHONY: clean

clean:
	rm *.o compress
