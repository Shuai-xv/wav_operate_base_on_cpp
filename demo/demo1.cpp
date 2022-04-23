#include "wav.h"
#include <string.h>
#include <stdio.h>


float Hn[] ={0.8783,-0.5806,0.6537,-0.3223,0.6577,-0.0582,0.2895,-0.2710,0.1278 ,
            -0.1508,0.0238,-0.1814,0.2519,-0.0396,0.0423,-0.0152,0.1664,-0.0245 ,
            0.1463,-0.0770,0.1304,-0.0148,0.0054,-0.0381,0.0374,-0.0329,0.0313 ,
            -0.0253,0.0552,-0.0369,0.0479,-0.0073,0.0305,-0.0138,0.0152,-0.0012 ,
            0.0154,-0.0092,0.0177,-0.0161,0.0070,-0.0042,0.0051,-0.0131,0.0059 ,
            -0.0041,0.0077,-0.0034,0.0074,-0.0014,0.0025,-0.0056,0.0028,-0.0005 ,
            0.0033,-0.0000,0.0022,-0.0032,0.0012,-0.0020,0.0017,-0.0022,0.0004,-0.0011,0,0};

int main(int argc, char const *argv[])
{//把两个音频合并
    if(argc!=4)
        return 0;
    wav_reader aud1(argv[1]);
    // wav_reader aud_echo(argv[2]);
    wav_writer aud_echo(argv[2],
                        aud1.get_channels(),
                        aud1.get_sample_rate(),
                        aud1.get_sample_width());
    wav_writer aud_combine(argv[3],
                        aud1.get_channels(),
                        aud1.get_sample_rate(),
                        aud1.get_sample_width());
    
    short lbuf[512]={0};
    short rbuf[512]={0};

    int delay=0.2*aud1.get_sample_rate();
    float alpha=0.4;
    for (int i = 0; i < delay; i++)
    {
        short tmp=0;
        aud_echo.write_data((char*)&tmp,NULL,2);
    }
    int cnt=0;
    while ((cnt=aud1.get_data((char*)lbuf,NULL,1024))==1024)
    {
        for (int i = 0; i < cnt/2; i++)
        {
            lbuf[i]=lbuf[i]*alpha;
        }
        
        aud_echo.write_data((char*)lbuf,NULL,1024);
    }
    aud_echo.~wav_writer();
    aud1.~wav_reader();
    wav_reader aud_echo_1(argv[2]);
    wav_reader aud1_1(argv[1]);

    while (1)
    {
        int len1=aud1_1.get_data((char*)lbuf,NULL,1024);
        int len2=aud_echo_1.get_data((char*)rbuf,NULL,1024);
        int len=len1>len2?len2/2:len1/2;
        for (int i = 0; i < len; i++)
        {
            short tmp=(lbuf[i]+rbuf[i]);
            aud_combine.write_data((char*)&tmp,NULL,2);
        }
        if(len1!=len2)
            break;
    }
    
    return 0;
}