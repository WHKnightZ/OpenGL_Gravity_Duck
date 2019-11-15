Mix_Music *Music_BackGround = NULL;
Mix_Chunk *Sound_Switch = NULL;
Mix_Chunk *Sound_Touch_Switch = NULL;
Mix_Chunk *Sound_Dead = NULL;
Mix_Chunk *Sound_Pick_Egg = NULL;

void Init_Sound() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != -1) {
        Music_BackGround = Mix_LoadMUS("Sounds/Background.mp3");
        Sound_Switch = Mix_LoadWAV("Sounds/Switch.wav");
        Sound_Touch_Switch = Mix_LoadWAV("Sounds/Touch_Switch.wav");
        Sound_Dead = Mix_LoadWAV("Sounds/Dead.wav");
        Sound_Pick_Egg = Mix_LoadWAV("Sounds/Pick_Egg.wav");
        if (!Mix_PlayingMusic())
            Mix_PlayMusic(Music_BackGround, -1);
    }
}
