import matplotlib.pyplot as plt
import numpy as np
import argparse
import sys
import os.path

# add file arg
parser = argparse.ArgumentParser(description='Convert bitmap image to raw bytes and write to file. Write to filename.bytes inside bytes folder')
parser.add_argument("file", help="filename of the bitmap image. Must be located in the images subfolder", type=str)
if len(sys.argv)==1:
    parser.print_help(sys.stderr)
    sys.exit(1)
args=parser.parse_args()

# check if file exists
if not os.path.isfile('images/'+args.file):
    sys.exit('ERROR: File does not exist in images directory')


#read img bytes into 2-d array
arr = plt.imread('images/'+args.file)

plt.imshow(arr)
plt.show()

#access pixel using [row,col] indexing
# indexing [0, :] would be entire first row

rootName = args.file.split(".")[0]
#open mode writing in binary with truncation of file first
fp = open('bytes/'+rootName+".bytes", 'wb')

#write bytes rows from array
for i in range(np.shape(arr)[0]):
    fp.write(arr[i,:])


fp.close()
