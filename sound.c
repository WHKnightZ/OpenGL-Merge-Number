Mix_Chunk* Sound[MAX_COMBO];
Mix_Chunk* Sound_Game_Over;

void Load_Sound()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != -1) {
        Sound[0] = Mix_LoadWAV("Sounds/Click.wav");
        Sound[1] = Mix_LoadWAV("Sounds/Combo_1.wav");
        Sound[2] = Mix_LoadWAV("Sounds/Combo_2.wav");
        Sound[3] = Mix_LoadWAV("Sounds/Combo_3.wav");
        Sound[4] = Mix_LoadWAV("Sounds/Combo_4.wav");
        Sound[5] = Mix_LoadWAV("Sounds/Combo_5.wav");
        Sound[6] = Mix_LoadWAV("Sounds/Combo_6.wav");
        Sound[7] = Mix_LoadWAV("Sounds/Combo_7.wav");
        Sound[8] = Mix_LoadWAV("Sounds/Combo_8.wav");
        Sound[9] = Mix_LoadWAV("Sounds/Combo_9.wav");
        Sound_Game_Over = Mix_LoadWAV("Sounds/Game_Over.wav");
    }
}

void Play_Sound(int n)
{
    Mix_PlayChannel(-1, Sound[n], 0);
}

void Play_Sound_Game_Over()
{
    Mix_PlayChannel(-1, Sound_Game_Over, 0);
}
