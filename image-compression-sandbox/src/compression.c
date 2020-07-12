#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../inc/compression.h"

// https://code.google.com/archive/p/lzfx/ lzfx library from google project
#include "../inc/lzfx.h"
//lz4
#include "../inc/lz4.h"
//zLib
#include "../inc/zlib.h"
#include "../inc/lzfP.h"
#include "../inc/lzf.h"


int main(int argc, char **argv) {

  if (argc != 2){
    fprintf( stderr, "ERROR: Invalid arguments\n"
      "Please provide only one argument; file name (including path) of byte file containing "
      "raw image bytes\n ex. 'bytes/img.bytes'\n");
    exit(EXIT_FAILURE);
  }

  if( access( argv[1], F_OK ) == -1 ) {
    fprintf(stderr, "ERROR: File not found in specified location\n");
    exit(EXIT_FAILURE);
  }

  // at this point, file name was provided and exists so open it up
  FILE *fp;
  char *buffer;
  unsigned int fileLen;

  fp = fopen(argv[1], "rb");  // Open as read in binary mode
  fseek(fp, 0, SEEK_END);     // seek pointer to EOF
  fileLen = ftell(fp);        // Get the current byte offset (length)
  rewind(fp);                 // Reset FP to start of file
  buffer = (char *)malloc(fileLen * sizeof(char)); // Malloc enough memory for the file
  fread(buffer, fileLen, 1, fp); // Read in the entire file into buffer
  fclose(fp); // Close the file

  printf("Original file size is %u bytes\n", fileLen);
  // now the images bytes are contained in a 1d array 'buffer' (aligned as row1, row2, row3, etc)
  int rc;
  int lz4OutSize;
  int zLibErr;
  char *outBuf_lzfx;
  char *outBuf_lz4;
  char *outBuf_zLib;
  unsigned int outSize = fileLen*2;
  unsigned long zLibOutSize = fileLen * 2;
  // we need the output (compressed) buffer to be larger just in case
  outBuf_lzfx = (char*)malloc(outSize * sizeof(char));
  outBuf_lz4 = (char*)malloc(outSize * sizeof(char));
  outBuf_zLib = (char*)malloc(zLibOutSize * sizeof(char));
  if (!outBuf_lzfx || !outBuf_lz4 || !outBuf_zLib) {
    fprintf(stderr, "Failed to malloc for output buffer\n");
    exit(EXIT_FAILURE);
  }
  //lzfx compression
  rc = lzfx_compress(buffer, fileLen, outBuf_lzfx, &outSize);

  printf("===============\nlzfx Compression\n===============\n");

  if (rc < 0) {
    // compression failed
    fprintf(stderr, "lzfx compression failed: buffer not modified. Error code %d\n", rc);
  }
  else {
    printf("lzfx compression was successful\n");
    //ratio = uncompressed / Compressed
    float compressionRatio = (float)fileLen / (float)outSize;
    printf("lzfx output size is %u bytes\n", outSize);
    printf("lzfx compression ratio: %f\n", compressionRatio);

    if(outSize > fileLen) {
      printf("lzfx compression resulted in larger output file\n");
    }
  }

  //lz4 compression
  lz4OutSize = LZ4_compress_default(buffer, outBuf_lz4, fileLen, outSize);

  printf("===============\nlz4 Compression\n===============\n");

  if (lz4OutSize < 1) {
    //compression Failed
    printf("lz4 compression failed\n");
  }
  else {
    printf("lz4 compression was successful\n");
    float lz4CompressionRatio = (float)fileLen / (float)lz4OutSize;
    printf("lz4 output size is %u bytes\n", lz4OutSize);
    printf("lz4 compression ratio: %f\n", lz4CompressionRatio);

    if (lz4OutSize > fileLen) {
      printf("lz4 compression resulted in larger output file\n");
    }
  }

  //zLib compressionRatio
  printf("===============\nzLib Compression\n===============\n");

  zLibErr = compress((Bytef*)outBuf_zLib, &zLibOutSize, (Bytef*)buffer, fileLen);

  if (zLibErr != Z_OK) {
    //compression Failed
    printf("zLib compression failed\n");
  }
  else {
    printf("zLib compression was successful\n");
    float zLibCompressionRatio = (float)fileLen / (float)zLibOutSize;
    printf("zLib output size is %lu bytes\n", zLibOutSize);
    printf("zLib compression ratio: %f\n", zLibCompressionRatio);

    if (zLibOutSize > fileLen) {
      printf("zLib compression resulted in larger output file\n");
    }
  }
  // http://software.schmorp.de/pkg/liblzf.html
  printf("===============\nlib lzf Compression\n===============\n");
  char* out2;
  outSize = fileLen*2;
  unsigned int rc2;
  out2= (char*)malloc(outSize * sizeof(char));
  rc2 = lzf_compress(buffer, fileLen, out2, outSize);

  if (rc2 == 0){
    fprintf(stderr, "lib lzf compression failed: buffer not modified.\n");
  }
  else{
    printf("lib lzf compression was successful\n");
    float compressionRatio = (float)fileLen / (float)rc2;
    printf("lib lzf output size is %u bytes\n", rc2);
    printf("lib lzf compression ratio: %f\n", compressionRatio);
    if(rc2 > fileLen) {
      printf("Compression resulted in larger output file\n");
    }
  }

  return 0;
}
