#include <iostream>
#include "../include/loadwav.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
int main(int argc, char **argv)
{
    load_wav res;
    Mix_Music *music = 0;
    // loadwave("./short.wav", res);
    music = Mix_LoadMUS("./short.mp3");
    if (!music)
    {
        printf("error");
    }
    printf("%d", music);

    return 0;
}