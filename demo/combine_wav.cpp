#include "wav.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{//把两个音频合并
    printf("combine 2 signal channel audio to double channel audio\n");
    if(argc!=3){
        printf("e.g. ./build/combine_wav test_datas/echo.wav ./test_datas/far_end.wav\n");
        printf("\t file ./combine.wav will be generated\n");
        return 0;
    }
    wav_reader aud1(argv[1]);
    wav_reader aud2(argv[2]);

    // 试着把两个单声道音频合成双声道的
    wav_writer aud_useless("./combine.wav",
                        2,
                        aud1.get_sample_rate(),
                        aud1.get_sample_width());
    short lbuf[512];
    short rbuf[512];
    while (1){
        int a1=aud1.get_data((char*)lbuf,NULL,512*2,nullptr);
        int a2=aud2.get_data((char*)rbuf,NULL,512*2,nullptr);
        if(a1==a2)
            aud_useless.write_data((char*)lbuf,(char*)rbuf,a1);
        if(a1==0)
            return 0;
        
    }
    return 0;
}