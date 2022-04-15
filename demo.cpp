#include "wav.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    if(argc!=2)
        return 0;
    wav_reader read(argv[1]);
    int lbuf[512];
    int rbuf[512];
    int cnt=0;
    int tmp;
    // int i=0;
    while(tmp=read.get_data(lbuf,rbuf,400)){
        // printf("i:%d,tmp=%d\n",++i,tmp);
        cnt+=tmp;
    }
    printf("cnt:%d\n",cnt*2);
    return 0;
}