#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../inc/compression.h"

// https://code.google.com/archive/p/lzfx/ lzfx library from google project
#include "../inc/lzfx.h"
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

  printf("File is %u bytes\n", fileLen);
  // now the images bytes are contained in a 1d array 'buffer' (aligned as row1, row2, row3, etc)
  int rc;
  char *outBuf;
  unsigned int outSize = fileLen*2;
  // we need the output (compressed) buffer to be larger just in case
  outBuf = (char*)malloc(outSize * sizeof(char));
  if (!outBuf) {
    fprintf(stderr, "Failed to malloc for output buffer\n");
    exit(EXIT_FAILURE);
  }
  rc = lzfx_compress(buffer, fileLen, outBuf, &outSize);

  if (rc < 0) {
    // compression failed
    fprintf(stderr, "lzfx compression failed: buffer not modified. Error code %d\n", rc);
  }
  else {
    printf("Compression was successful\n");
    //ratio = uncompressed / Compressed
    float compressionRatio = (float)fileLen / (float)outSize;
    printf("Output size is %u bytes\n", outSize);
    printf("Compression ratio: %f\n", compressionRatio);

    if(outSize > fileLen) {
      printf("Compression resulted in larger output file\n");
    }
  }

  // http://software.schmorp.de/pkg/liblzf.html
  printf("\nTrying lib lzf library now\n");
  char* out2;
  outSize = fileLen*2;
  unsigned int rc2;
  out2= (char*)malloc(outSize * sizeof(char));
  rc2 = lzf_compress(buffer, fileLen, out2, outSize);

  if (rc2 == 0){
    fprintf(stderr, "lib lzf compression failed: buffer not modified.\n");
  }
  else{
    printf("Compression was successful\n");
    float compressionRatio = (float)fileLen / (float)rc2;
    printf("Output size is %u bytes\n", rc2);
    printf("Compression ratio: %f\n", compressionRatio);
    if(rc2 > fileLen) {
      printf("Compression resulted in larger output file\n");
    }
  }



  return 0;
}
