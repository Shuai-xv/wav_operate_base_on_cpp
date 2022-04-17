#include "wav.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{//把两个音频合并
    if(argc!=4)
        return 0;
    wav_reader aud1(argv[1]);
    wav_reader aud2(argv[2]);
    wav_writer aud_out(argv[3],
                        aud1.get_channels(),
                        aud1.get_sample_rate(),
                        aud1.get_sample_width());
    // 试着把两个单声道音频合成双声道的
    wav_writer aud_useless("./useless.wav",
                        2,
                        aud1.get_sample_rate(),
                        aud1.get_sample_width());
    short lbuf[512];
    short rbuf[512];
    int sum=0;
    int cnt=0;
    while (1){
        int a1=aud1.get_data((char*)lbuf,NULL,512*2);
        int a2=aud2.get_data((char*)rbuf,NULL,512*2);
        // printf("===%p===%p\n",&aud1,&aud2);
        // break;
        int len=a1>a2?a2:a1;
        aud_useless.write_data((char*)lbuf,(char*)rbuf,len);
        for (int i=0;i<len;i++){
            lbuf[i]=(lbuf[i]+rbuf[i])/4;
        }
        sum+=len;
        aud_out.write_data((char*)lbuf,NULL,len);
        cnt++;
        if(len<512*2){
            printf("======sum:%d,(%d,%d),%d\n",sum,a1,a2,cnt);
            break;
        }
    }
    aud_useless.~wav_writer();
    wav_reader wtmp("./useless.wav");
    return 0;
}