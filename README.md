# csc4420FinalProject
Final Project for csc4420 - a stand alone rc4 implementation and integration of that implementation into s3fs

## Instructions
Clone this repository, create the standalone program and build the s3fs program

## StandAlone Instalation
navigate to where you cloned the program and navigate to standAlone_rc4
use command "gcc rc4StandAlone.c -o rc4StandAlone.out" to create rc4StandAlone.out

correct usage
./rc4StandAlone.out inputfile outputfile

there are no salt options in this implementation or options for encrypt or decrypt, uses the stand rc4 xor to determine what to do.

## S3fs Installation
navigate to the compilation.md file and follow the directions there, it is the same process as installing the original s3fs program. 

* ./autogen.sh
* ./configure
* make
* sudo make install
