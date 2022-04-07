#include <stdio.h>
#include "wav_reader.h"
#include <string.h>

wav_reader::wav_reader(const char* fn)
{
    mem_cnt=0;
    fp=fopen(fn,"rb");
    if(fp==nullptr){
        printf("open failed\n");
        return;
    }
    fread(&header,1,sizeof(header),fp);
    if(strncasecmp(header.riffType,"RIFF",4) || strncasecmp(header.waveType,"wave",4) || strncasecmp(header.dataType,"data",4)) {
        printf("error:not wav file or file is broken\n");
        fclose(fp);
        fp=nullptr;
        return;
    }
    if(header.compressionCode!=1){
        printf("only support WAVE_FORMAT_PCM!\n");
        fclose(fp);
        fp=nullptr;
        return;
    }
    printf("riffType:%4.4s\n",header.riffType);    
    printf("riffSize:%d\n",header.riffSize);       
    printf("waveType:%4.4s\n",header.waveType);    
    printf("formatType:%4.4s\n",header.formatType);  
    printf("formatSize:%d\n",header.formatSize);
    printf("compressionCode:%d\n",header.compressionCode);
    printf("numChannels:%d\n",header.numChannels);    
    printf("sampleRate:%d\n",header.sampleRate);
    printf("bytesPerSecond:%d\n",header.bytesPerSecond);
    printf("blockAlign:%d\n",header.blockAlign);
    printf("bitsPerSample:%d\n",header.bitsPerSample);
    printf("dataType:%4.4s\n",header.dataType);    
    printf("dataSize:%d\n",header.dataSize);       
}

wav_reader::~wav_reader()
{
    if(fp)
        fclose(fp);
    fp=nullptr;
}

int wav_reader::get_channels()
{
    return fp?header.numChannels:0;
}
int wav_reader::get_sample_rate()
{
    return fp?header.sampleRate:0;
}
int wav_reader::get_frame_cnt()
{
    return fp?header.dataSize:0;
}
int wav_reader::get_data(int frame_cnt,int*buf)
{
    if(fp){
        int size=(header.bitsPerSample+7)/8;
        int cnt=fread(buf,size,frame_cnt,fp);
        mem_cnt+=cnt*size;
        int tmp=cnt;
        if(mem_cnt>header.dataSize){
            cnt-=(mem_cnt-header.dataSize)/size;
            printf("====:%u\n",((int)mem_cnt-header.dataSize)/size);
            memset(buf+cnt,0,frame_cnt-cnt);
            mem_cnt-=tmp*size;
            mem_cnt+=cnt*size;
        }
        // printf("memcnt=%d,size=%d,cnt:%d\n",mem_cnt,size,cnt);
        return cnt;
    }
    return 0;
}


int main(int argc, char const *argv[])
{
    if(argc!=2)
        return 0;
    wav_reader read(argv[1]);
    int buf[512];
    int cnt=0;
    int tmp;
    int i=0;
    while(tmp=read.get_data(400,buf)){
        // printf("i:%d,tmp=%d\n",++i,tmp);
        cnt+=tmp;
    }
    printf("cnt:%d\n",cnt*2);
    return 0;
}
