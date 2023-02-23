#include "../include/mp3.h"
#define mp3 mp3
bool loadmp3(const char *filname, mp3 mp3)
{
    FILE *fptr;
    /* ftell tell me position of pointer
     * fseek set pointer to position i wont
     */
    fptr = fopen(filname, "rb");
    // fseek(fptr, 4, SEEK_SET);
    fread(mp3.id3, 5, 1, fptr);
    if (mp3.id3[0] != 'I', mp3.id3[1] != 'D', mp3.id3[2] != '3')
    {
        printf("error  not do");
        fclose(fptr);
        exit(0);
    }
    else
    {
        printf("reale work %c %c\n", mp3.id3[3], mp3.id3[4]);
        int len = ftell(fptr);
        printf("file pointer %d \n", len);
    }
    int t = 0;
    while (t)
    {
        fread(mp3.sync, 3, 1, fptr);
        // mp3.sync[0] != 'f' | mp3.sync[2] != 'f' | mp3.sync[2] != 'f')
        if (mp3.sync[0] != 'f', mp3.sync[2] != 'f', mp3.sync[2] != 'f')
        {
            printf("we fond %c   %c  %c \n", mp3.sync[0], mp3.sync[1], mp3.sync[2]);
            t = 1;
        }
        else
        {
            t = 1;
            printf("cannot \n");
        }
    }

    return true;
}

int find_sync(FILE *fp)
{
    int position;

    return position;
}