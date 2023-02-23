#ifndef MP3_H
#define MP3_H
#include <iostream>
typedef struct mp3
{
    /* header bits or 4 bytes */
    unsigned char id3[3];
    unsigned char sync[4];

    /* side information 17 bytes if it is a single channel or 32 bytes if */

    /* main data */

    /*  Ancillary data */
} mp3;
int find_sync(FILE *fp);
bool loadmp3(const char *filname, mp3 mp3);
#endif