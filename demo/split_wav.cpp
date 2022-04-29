#include "wav.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("Convert an audio with 2 channels to 2 audios with signal channel\n");
    if(argc!=2){
        printf("e.g. ./build/split_wav ./combine.wav\n");
        printf("\t file ./left.wav and ./right.wav will be generated\n");
        return 0;
    }
    wav_reader aud1(argv[1]);
    if(aud1.get_channels()!=2){
        printf("please input a file with 2 channels");
        return -1;
    }
    wav_writer left("./left.wav",1,aud1.get_sample_rate(),aud1.get_sample_width());
    wav_writer right("./right.wav",1,aud1.get_sample_rate(),aud1.get_sample_width());

    short lbuf[128],rbuf[128];
    while (aud1.get_data((char*)lbuf,(char*)rbuf,sizeof(lbuf),nullptr)>0){
        left.write_data((char*)lbuf,nullptr,sizeof(lbuf),nullptr);
        right.write_data((char*)rbuf,nullptr,sizeof(lbuf),nullptr);
        // return 0;
    }
    return 0;
}