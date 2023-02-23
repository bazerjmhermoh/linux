#ifndef LOAD_WAV_H
#define LOAD_WAV_H
#include <iostream>

typedef struct LOADWAV
{
    char RIIF[4];
    char format[4];
    unsigned int overallsize;
    char fmtchunk[4];
    unsigned int subchunksize;
    unsigned int formatype;
    char formatname[10];
    unsigned int numofchannels;
    unsigned int samplerate;
    unsigned int Byterate;
    unsigned int block_align;           // NumChannels * BitsPerSample/8
    unsigned int bits_per_sample;       // bits per sample, 8- 8bits, 16- 16 bits etc
    unsigned char data_chunk_header[4]; // DATA string or FLLR string
    unsigned int data_size;
    int data;
    long sizeofeachsample;
    long byteinchannal;
} load_wav;

bool loadwave(const char *filename, load_wav res);
#endif
