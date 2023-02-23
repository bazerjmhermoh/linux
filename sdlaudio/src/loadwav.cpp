#include "../include/loadwav.h"
#include <string.h>
bool loadwave(const char *filename, load_wav res)
{
    FILE *fptr;

    if ((fptr = fopen(filename, "rb")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    fread(res.RIIF, 1, 4, fptr);
    if (res.RIIF[0] != 'R', res.RIIF[1] != 'I', res.RIIF[2] != 'F', res.RIIF[3] != 'F')
    {
        fclose(fptr);
        exit(0);
    }
    // over all size i do not now right now
    char over[4];
    fread(over, 4, 1, fptr);
    // convert little endian to big endian 4 byte int
    res.overallsize = over[0] | (over[1] << 8) | (over[2] << 16) | (over[3] << 24);

    printf("(5-8) Overall size: bytes:%u, Kb:%u \n", res.overallsize, res.overallsize / 1024);

    fread(res.format, 4, 1, fptr);
    if (res.format[0] != 'W', res.format[1] != 'A', res.format[2] != 'V', res.format[3] != 'E')
    {
        printf("min\n");
        fclose(fptr);
        exit(0);
    }
    fread(res.fmtchunk, 4, 1, fptr);
    if (res.fmtchunk[0] != 'f', res.fmtchunk[1] != 'm', res.fmtchunk[2] != 't', res.fmtchunk[3] != ' ')
    {
        printf("this is the error is == %s ====\n", res.fmtchunk);
        fclose(fptr);
        exit(0);
    }
    char sub[4];
    fread(sub, 4, 1, fptr);
    res.subchunksize = sub[0] | (sub[1] << 8) | (sub[2] << 16) | (sub[3] << 24);
    printf("sub chunk %u", res.subchunksize);
    char buffer2[2];
    fread(buffer2, sizeof(buffer2), 1, fptr);

    res.formatype = buffer2[0] | (buffer2[1] << 8);

    printf("\n %u  \n", res.formatype);

    if (res.formatype == 1)
        strcpy(res.formatname, "PCM");
    else if (res.formatype == 6)
        strcpy(res.formatname, "A-law");
    else if (res.formatype == 7)
        strcpy(res.formatname, "Mu-law");
    printf("format name %s\n", res.formatname);

    /*********************************************************/
    /*********************************************************/
    unsigned char buffer4[4];
    fread(buffer2, sizeof(buffer2), 1, fptr);
    printf("%u %u n", buffer2[0], buffer2[1]);

    res.numofchannels = buffer2[0] | (buffer2[1] << 8);
    printf("(23-24) Channels: %u \n", res.numofchannels);

    fread(buffer4, sizeof(buffer4), 1, fptr);
    printf("%u %u %u %un", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    res.samplerate = buffer4[0] |
                     (buffer4[1] << 8) |
                     (buffer4[2] << 16) |
                     (buffer4[3] << 24);

    printf("\n(25-28) Sample rate: %u\n", res.samplerate);

    fread(buffer4, sizeof(buffer4), 1, fptr);
    printf("%u %u %u %un", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    res.Byterate = buffer4[0] |
                   (buffer4[1] << 8) |
                   (buffer4[2] << 16) |
                   (buffer4[3] << 24);
    printf("\n(29-32) Byte Rate: %u , Bit Rate:%u\n", res.Byterate, res.Byterate * 8);

    //////////////////////////////////////////////////////////////////////////////
    fread(buffer2, sizeof(buffer2), 1, fptr);
    printf("%u %u \n", buffer2[0], buffer2[1]);

    res.block_align = buffer2[0] |
                      (buffer2[1] << 8);
    printf("(33-34) Block Alignment: %u \n", res.block_align);

    fread(buffer2, sizeof(buffer2), 1, fptr);
    printf("%u %u \n", buffer2[0], buffer2[1]);

    res.bits_per_sample = buffer2[0] |
                          (buffer2[1] << 8);
    printf("(35-36) Bits per sample: %u \n", res.bits_per_sample);

    fread(res.data_chunk_header, sizeof(res.data_chunk_header), 1, fptr);
    printf("(37-40) Data Marker: %s \n", res.data_chunk_header);

    fread(buffer4, sizeof(buffer4), 1, fptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    res.data_size = buffer4[0] |
                    (buffer4[1] << 8) |
                    (buffer4[2] << 16) |
                    (buffer4[3] << 24);
    printf("(41-44) Size of data chunk: %u \n", res.data_size);
    /**********************************************************************/

    // calculate no.of samples
    long num_samples = (8 * res.data_size) / (res.numofchannels * res.bits_per_sample);
    printf("Number of samples:%lu \n", num_samples);

    long size_of_each_sample = (res.numofchannels * res.bits_per_sample) / 8;
    printf("Size of each sample:%ld bytes\n", size_of_each_sample);
    res.sizeofeachsample = size_of_each_sample;
    // calculate duration of file
    float duration_in_seconds = (float)res.overallsize / res.Byterate;
    printf("Approx.Duration in seconds=%fn", duration_in_seconds);
    // printf("Approx.Duration in h:m:s=%sn", seconds_to_time(duration_in_seconds));

    /************************************************************/
    if (res.formatype == 1)
    { // PCM
        printf("Dump sample data? Y/N?");
        char c = 'n';
        scanf("%c", &c);
        if (c == 'Y' || c == 'y')
        {
            long i = 0;
            char data_buffer[size_of_each_sample];
            int size_is_correct = 1;

            // make sure that the bytes-per-sample is completely divisible by num.of channels
            long bytes_in_each_channel = (size_of_each_sample / res.numofchannels);
            res.byteinchannal = bytes_in_each_channel;
            if ((bytes_in_each_channel * res.numofchannels) != size_of_each_sample)
            {
                printf("Error: %ld x %ud <> %ldn", bytes_in_each_channel, res.numofchannels, size_of_each_sample);
                size_is_correct = 0;
            }

            if (size_is_correct == 1)
            {
                // the valid amplitude range for values based on the bits per sample
                long low_limit = 0l;
                long high_limit = 0l;

                switch (res.bits_per_sample)
                {
                case 8:
                    low_limit = -128;
                    high_limit = 127;
                    break;
                case 16:
                    low_limit = -32768;
                    high_limit = 32767;
                    break;
                case 32:
                    low_limit = -2147483648;
                    high_limit = 2147483647;
                    break;
                }

                printf("\nn.Valid range for data values : %ld to %ld \n", low_limit, high_limit);
                for (i = 1; i <= num_samples; i++)
                {
                    printf("\n==========Sample %ld =============", i); // num_samples  / %ld
                    size_t read = fread(data_buffer, sizeof(data_buffer), 1, fptr);
                    if (read == 1)
                    {

                        // dump the data read
                        unsigned int xchannels = 0;
                        int data_in_channel = 0;
                        int offset = 0; // move the offset for every iteration in the loop below
                        for (xchannels = 0; xchannels < res.numofchannels; xchannels++)
                        {
                            printf("\nChannel******  %d : ", (xchannels + 1));
                            // convert data from little endian to big endian based on bytes in each channel sample
                            if (bytes_in_each_channel == 4)
                            {
                                data_in_channel = (data_buffer[offset] & 0x00ff) |
                                                  ((data_buffer[offset + 1] & 0x00ff) << 8) |
                                                  ((data_buffer[offset + 2] & 0x00ff) << 16) |
                                                  (data_buffer[offset + 3] << 24);
                            }
                            else if (bytes_in_each_channel == 2)
                            {
                                data_in_channel = (data_buffer[offset] & 0x00ff) |
                                                  (data_buffer[offset + 1] << 8);
                            }
                            else if (bytes_in_each_channel == 1)
                            {
                                data_in_channel = data_buffer[offset] & 0x00ff;
                                data_in_channel -= 128; // in wave, 8-bit are unsigned, so shifting to signed
                            }

                            offset += bytes_in_each_channel;
                            printf("%d ", data_in_channel);
                            res.data = data_in_channel;

                            // check if value was in range
                            if (data_in_channel < low_limit || data_in_channel > high_limit)
                                printf("**value out of rangen");

                            // printf(" | ");
                        }

                        // printf("n");
                    }
                    else
                    {
                        printf("Error reading file. %d bytes\n", read);
                        break;
                    }
                }
            }
        }

        return true;
    }
}