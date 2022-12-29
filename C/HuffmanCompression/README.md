# Huffman Compression
This program is an implementation of the lossless data compression algorithm created by David Huffman. The algorithm uses frequencies of symbols to assign variable-length prefix codes. This program will accept a file as a command line argument. Huffman encodings are then assigned to all of the symbols in this file. I used a prority queue in the form of a min-heap to select the smallest frequencies before transforming the heap into a Huffman tree. The bottom up heap construction algorithm was used to transform a complete binary tree into a min-heap. 
# Instructions for use
````
make
./main [FILE]
````
# Credits
#### Parker Hendry
#### hendryp2@winthrop.edu
