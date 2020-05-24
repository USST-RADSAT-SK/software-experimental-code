# Image Compression Sandbox

The following C program is designed to test different image compression algorithms and libraries on sample bitmap images.
A python utility program is included to easily extract the image data and convert to raw bytes.

# Python utility
Matplotlib is required to run. This utility is set up to run from the command line, taking an image file name as an argument. The image must be 8-bit greyscale bitmap only. The raw bytes will be extracted and written to the file bytes/imageName.bytes. This file will be used by the C program.

# C program
The C program has only been tested on Ubuntu. Please build using the make file provided. lzfx compression library is included as a sample. The C program is also meant to be run from the command line, taking the byte file name as an argument. This byte file must be produced by the python utility above. 
