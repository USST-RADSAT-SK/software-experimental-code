
/**
 * @brief This section of the code is responsible for determining if a photo
 *         taken by the CubeSense CubeSat camera is worth the downlink. 
 * 
 *         This code will take in the byte array the CubeSense CubeSat 
 *          Camera produces by grabbing it from the CubeSense CubeSat 
 *          memory. After grabbing the image data, it will convert all the 
 *          bytes into a histrogram with a set amount of bins. These bins then
 *          will be used to calculate a variance value of the middle bin(s). If
 *          the middle bin has a large variance it will determined as a properally
 *          exposed photo and will pass the quailty check
 * 
 * @header "the/include/image_quality.c"
 * @param c must include the byte array from the CubeSense memory
 * @pre the pre condiction would be that a photo must be taken before hand and the cubesense memory is not empty
 * @post No change in memory after post as all variables will be de allocated
 * @return will return a true (past quality check) or a false (failed quality check)
 */

//TODO: Make a time out feature

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//TODO: Can a determination for variance range between 4000-11000, pass those values

void variant(unsigned char *image_buffer, long fileLen){
   double sum;
   float X_bar;
   float variance;
   for ( int i = 0; i < fileLen; i ++){
        sum = sum + image_buffer[i];
    }
    printf("Sum is: %f\n", sum);

   X_bar = sum/fileLen;

   printf("xbar is: %f \n",X_bar);

    float sum_x_minus_xbar = 0;

    for ( int i = 0; i < fileLen; i ++){
        sum_x_minus_xbar = sum_x_minus_xbar + ( pow((image_buffer[i] - X_bar),2));
    }
    printf("Sum numerator is: %f \n", sum_x_minus_xbar);

    variance = sum_x_minus_xbar /(fileLen-1);

    //if variance between range
    double highest = 11211.87;
    double lowest = 4897.86;


    printf("Variance: %f\n", variance);
    printf("S.D: %f\n", sqrt(variance));
    printf("File length is: %ld", fileLen);

    // check if it is a good picture
    int check = 0;
    if (lowest <= variance && variance <= highest){
        check = 1;}

}


int main (int argc, char *argv[]) {

/* takes two command line arguments; the program name and one input file. 
Extracts data from file into array. Displays final results. */


     if (argc != 2) {
        printf("Usage: <exe_name> <image_file>\n");
        return 1;
        }

    
   
    long filelen;
    FILE *fileptr = fopen(argv[1], "rb");  // Open the file in binary mode

    // Make sure file opened successfully.
    if(fileptr==NULL) {printf("Unable to open %s for reading.\n", argv[1]); return 1;}


    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file
    unsigned char *buffer = (unsigned char *)malloc(filelen * sizeof(unsigned char)); // Enough memory for the file
    fread(buffer, filelen, 1, fileptr);   // Read in the entire file


    variant(buffer, filelen);

    free(buffer);
    fclose(fileptr);

}

// 50 Bad light argument files --> average variance ---> low end of the accepted range
// 50 Bad dark argument files --> average variance ---> high end of the accepted range