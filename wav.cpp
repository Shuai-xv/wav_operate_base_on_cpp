#include "wav.h"
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
int wav_reader::get_data(void*lbuf,void*rbuf,int buf_len)
{
    if(fp){
        char*buf;
        if(header.numChannels==1){
            if(lbuf==nullptr){
                printf("lbuf is null\n");
                return -1;
            }
            buf=(char*)lbuf;
        }else if(header.numChannels==2){
            if(lbuf==nullptr||rbuf==nullptr){
                printf("rbuf or lbuf is null\n");
                return -1;
            }
            buf=new char[header.blockAlign*buf_len];
        }
        // int size=(header.bitsPerSample+7)/8;
        int cnt=fread(buf,header.blockAlign,buf_len,fp);
        mem_cnt+=cnt*header.blockAlign;
        // int tmp=cnt;
        if(mem_cnt>header.dataSize){
            cnt-=(mem_cnt-header.dataSize)/header.blockAlign;
            // printf("====:%u\n",((int)mem_cnt-header.dataSize)/size);
            // memset(buf+cnt,0,buf_len-cnt);
            // mem_cnt-=tmp*size;
            // mem_cnt+=cnt*size;
            mem_cnt=header.dataSize;
        }
        if(1==header.numChannels){
            memcpy(lbuf,buf,cnt);
        }
        else if(header.numChannels==2){
            char*tlbuf=(char*)lbuf;
            char*trbuf=(char*)rbuf;
            char*sbuf=buf;
            for (int i = 0; i < cnt; i++)
            {
                memcpy(tlbuf,buf,header.blockAlign/2);
                tlbuf+=header.blockAlign/2;
                buf+=header.blockAlign/2;
                memcpy(trbuf,buf,header.blockAlign/2);
                trbuf+=header.blockAlign/2;
                buf+=header.blockAlign/2;
            }
            delete sbuf;
            // printf("delete;\n");
        }
        // printf("memcnt=%d,size=%d,cnt:%d\n",mem_cnt,size,cnt);
        return cnt;
    }
    return -1;
}

wav_writer::wav_writer(const char* fn){
    int mem_cnt=0;
    fp=fopen(fn,"wb");
    if(fp==nullptr){
        printf("open failed\n");
        return;
    }
    strncpy(header.riffType,"RIFF",4);    
    header.riffSize=36;       
    strncpy(header.waveType,"WAVE",4);
    strncpy(header.formatType,"fmt ",4);  
    header.formatSize=16;     
    header.compressionCode=1;
    header.numChannels=-1;    
    header.sampleRate=-1;     
    header.bytesPerSecond=-1; 
    header.blockAlign=-1;     
    header.bitsPerSample=-1;  
    strncpy(header.dataType,"data",4);    
    header.dataSize=0;       
}
wav_writer::wav_writer(const char* fn,int nchannel,int fs,int samplewid){
    int mem_cnt=0;
    fp=fopen(fn,"wb");
    if(fp==nullptr){
        printf("open failed\n");
        return;
    }
    strncpy(header.riffType,"RIFF",4);    
    header.riffSize=36;       
    strncpy(header.waveType,"WAVE",4);
    strncpy(header.formatType,"fmt ",4);  
    header.formatSize=16;     
    header.compressionCode=1;
    header.numChannels=nchannel;    
    header.sampleRate=fs; 
    header.blockAlign=samplewid/8*nchannel;     
    header.bitsPerSample=samplewid;
    header.bytesPerSecond=fs*header.blockAlign; 
    strncpy(header.dataType,"data",4);    
    header.dataSize=0;       
}
wav_writer::~wav_writer(){
    if(fp){
        fclose(fp);
    }
    fp=nullptr;
}
int wav_writer::set_channels(int n){
    header.numChannels=n;
    return 1;
}
int wav_writer::set_sample_rate(int fs){
    header.sampleRate=fs;
    return 1;
}
int wav_writer::set_sample_width(int wid){
    header.bitsPerSample=wid;
    header.blockAlign=wid/2;
    return 1;
}
int wav_writer::write_data(void*left_buf,void*right_buf,int buf_len){
    if(fp==nullptr){
        printf("file handle is close\n");
        return -1;
    }
    void*buf=nullptr;
    if(left_buf==nullptr and right_buf==nullptr)
        return -1;
    if(header.sampleRate==-1){
        printf("please set sampleRate\n");
        return -1;
    }
    if(header.blockAlign==-1){
        printf("please set bitsPerSample\n");
        return -1;
    }
    if(header.numChannels==-1){
        printf("please set numChannels\n");
        return -1;
    }else if(header.numChannels==1){
        buf=left_buf?left_buf:right_buf;
    }else if(header.numChannels==2){
        header.blockAlign*=2;
        if(left_buf==nullptr||right_buf==nullptr){
            printf("buf is null\n");
            return -1;
        }
        buf=new char[buf_len*header.blockAlign];
        char*tmp=(char*)buf;
        char*ltmp=(char*)left_buf;
        char*rtmp=(char*)right_buf;
        for (int i = 0; i < buf_len; i++)
        {
            memcpy(tmp,ltmp,header.blockAlign/2);
            tmp+=header.blockAlign/2;
            ltmp+=header.blockAlign/2;
            memcpy(tmp,rtmp,header.blockAlign/2);
            tmp+=header.blockAlign/2;
            rtmp+=header.blockAlign/2;
        }
        buf_len*=2;
    }else{
        printf("error numChannels\n");
        return -1;
    }
    header.bytesPerSecond=header.sampleRate*header.blockAlign;
    header.dataSize=buf_len;
    header.riffSize+=buf_len;
    fwrite(&header,1,sizeof(header),fp);
    if(header.numChannels==2)
        delete buf;
    return fwrite(buf,buf_len,header.blockAlign,fp);
}