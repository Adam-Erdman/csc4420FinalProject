//Adam Erdman - dz6416
//Csc4420 OS Final
//RC4 Standalone program

// Program to encrypt and decrypt files
// utilizing stream cipher implementation
// #include "rc4_enc.c"
// #include "rc4_skey.c"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define KEY "csc4420"
#define KEY_LENGTH (sizeof(KEY)-1)
#define RC4_INT unsigned int


typedef struct rc4_key_st {
    RC4_INT x, y;
    RC4_INT data[256];
} RC4_KEY;

void RC4_set_key(RC4_KEY *key, int len, const unsigned char *data)
{
    register RC4_INT tmp;
    register int id1, id2;
    register RC4_INT *d;
    unsigned int i;

    d = &(key->data[0]);
    key->x = 0;
    key->y = 0;
    id1 = id2 = 0;

#define SK_LOOP(d,n) { \
                tmp=d[(n)]; \
                id2 = (data[id1] + tmp + id2) & 0xff; \
                if (++id1 == len) id1=0; \
                d[(n)]=d[id2]; \
                d[id2]=tmp; }

    for (i = 0; i < 256; i++)
        d[i] = i;
    for (i = 0; i < 256; i += 4) {
        SK_LOOP(d, i + 0);
        SK_LOOP(d, i + 1);
        SK_LOOP(d, i + 2);
        SK_LOOP(d, i + 3);
    }
}

void RC4(RC4_KEY *key, size_t len, const unsigned char *indata,
         unsigned char *outdata)
{
    register RC4_INT *d;
    register RC4_INT x, y, tx, ty;
    size_t i;

    x = key->x;
    y = key->y;
    d = key->data;

#define LOOP(in,out) \
                x=((x+1)&0xff); \
                tx=d[x]; \
                y=(tx+y)&0xff; \
                d[x]=ty=d[y]; \
                d[y]=tx; \
                (out) = d[(tx+ty)&0xff]^ (in);

    i = len >> 3;
    if (i) {
        for (;;) {
            LOOP(indata[0], outdata[0]);
            LOOP(indata[1], outdata[1]);
            LOOP(indata[2], outdata[2]);
            LOOP(indata[3], outdata[3]);
            LOOP(indata[4], outdata[4]);
            LOOP(indata[5], outdata[5]);
            LOOP(indata[6], outdata[6]);
            LOOP(indata[7], outdata[7]);
            indata += 8;
            outdata += 8;
            if (--i == 0)
                break;
        }
    }
    i = len & 0x07;
    if (i) {
        for (;;) {
            LOOP(indata[0], outdata[0]);
            if (--i == 0)
                break;
            LOOP(indata[1], outdata[1]);
            if (--i == 0)
                break;
            LOOP(indata[2], outdata[2]);
            if (--i == 0)
                break;
            LOOP(indata[3], outdata[3]);
            if (--i == 0)
                break;
            LOOP(indata[4], outdata[4]);
            if (--i == 0)
                break;
            LOOP(indata[5], outdata[5]);
            if (--i == 0)
                break;
            LOOP(indata[6], outdata[6]);
            if (--i == 0)
                break;
        }
    }
    key->x = x;
    key->y = y;
}

int main (int argc, char *argv[]) {

    if (argc < 3){
    perror("Incorrect argument count, format is: ./rc4StandAlone.c fileToBeCrypted outputfile ");
    return 1;
  }

  int n;
  int fd=open(argv[1], O_RDWR);
  int fd2=open(argv[2], O_CREAT | O_RDWR, 0777);
  char sendToRc4[1024];

   if (fd==0){
     perror("open failed");
     return 1;
   }


   char buffer[1024];
RC4_KEY key;
RC4_set_key(&key, KEY_LENGTH, (const unsigned char*)KEY);

 while((n = read(fd, buffer, 1024)) >  0){


   RC4(&key, n, buffer, sendToRc4);
   write(fd2, sendToRc4, n); //Write ouput buffer to the file
 }


 if(n<0){
   perror("fail");
   return 1;
 }
 close(fd);




}
