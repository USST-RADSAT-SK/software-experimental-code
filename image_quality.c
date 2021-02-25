
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


#include <stdio.h>
#include <stdlib.h>


#define BINS 5  // number of bins to hold each set of values. 


/******* This could be a downside: From calculation limit array values done below, it is discovered that the higher the number of bins, the greater the values displayed. Less bins will display less values. ***********/


void sort_display(unsigned char* buffer, long filelen) {

/* Functions take an array of bytes (char datatype) and the filelen. Each char represent a value from 0 to 255. Chars will be sorted into bins numerically.*/


/* Values are represented as unsigned characters ranging 0 to 255. */

       unsigned char one_bin = 255/BINS;               // split bins into limit for one bin

        int limit[BINS];                      // create array for all the limits

        for (int m=0; m<BINS; m++) {
            
                limit[m] = -128 + (one_bin * m) ;        // calculate values for each limit and store in array
        }

        
        // For each bin, go through data, sort into array
        int binArray[BINS];

        for (int k=0; k<BINS; k++) {

            for(int j=0; j<filelen; j++) {
    
               if((buffer[j] > limit[k]) && (buffer[j] <= limit[k+1])) {                                   
                  binArray[BINS]++; 
               } 
            }
        }
}

void calculate(unsigned char* array){

}


// int main (int argc, char *argv[]) {

// /* takes two command line arguments; the program name and one input file. 
// Extracts data from file into array. Displays final results. */


//      if (argc != 2) {
//         printf("Usage: <exe_name> <image_file>\n");
//         return 1;
//         }

    
   
//     long filelen;
//     FILE *fileptr = fopen(argv[1], "rb");  // Open the file in binary mode

//     // Make sure file opened successfully.
//     if(fileptr==NULL) {printf("Unable to open %s for reading.\n", argv[1]); return 1;}


//     fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
//     filelen = ftell(fileptr);             // Get the current byte offset in the file
//     rewind(fileptr);                      // Jump back to the beginning of the file
//     unsigned char *buffer = (unsigned char *)malloc(filelen * sizeof(unsigned char)); // Enough memory for the file
//     fread(buffer, filelen, 1, fileptr); // Read in the entire file


//     sort_display(buffer, filelen);

//     free(buffer);
//     fclose(fileptr); // Close the file

//     }

