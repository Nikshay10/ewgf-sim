#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct DirectionalTextures{
    SDL_Texture* u;
    SDL_Texture* d;
    SDL_Texture* f;
    SDL_Texture* b;
    SDL_Texture* ub;
    SDL_Texture* uf;
    SDL_Texture* df;
    SDL_Texture* db;
    SDL_Texture* two;
};

struct EWGFState{
    int state = 0;
    int frame = 0;
    int dfStart = -1;
    bool ewgf = false;
    bool wgf = false;
};

EWGFState ewgfState;

bool isF(int d)  { return d == 6; }
bool isN(int d)  { return d == 5; }
bool isD(int d)  { return d == 2; }
bool isDF(int d) { return d == 3; }

void updateEWGF(int direction, bool two){
    ewgfState.ewgf = false;
    ewgfState.wgf = false;
    ewgfState.frame++;

    switch(ewgfState.state){
        case 0: // Forward tap
            if(isF(direction)){ 
                ewgfState.state = 1;
                ewgfState.frame = 0;
            }
            break;
        case 1: // Forward window 
            if(ewgfState.frame > 6){
                ewgfState.state = 0;
                break;
            }
            if(isN(direction)){ 
                ewgfState.state = 2;
                ewgfState.frame = 0;
            }
            else if(!isF(direction)){ // Checking for any direction other than neutral or forward
                ewgfState.state = 0;
            }
            break;
        case 2: // Neutral window
            if (ewgfState.frame > 6) {
                ewgfState.state = 0; // too slow
                break;
            }
            if (isD(direction)) {
                ewgfState.state = 3;
                ewgfState.frame = 0;
            }
            break;
        case 3: // downforward window
            if(ewgfState.frame > 4){ // too slow
                ewgfState.state = 0;
                break;
            }
            if(isDF(direction)){
                ewgfState.dfStart = ewgfState.frame;
                ewgfState.state = 4;
                ewgfState.frame = 0;
            }
            break;
        case 4:
            if(ewgfState.frame > 4){
                ewgfState.state = 0;
                break;
            }
            if(two && ewgfState.dfStart <=2)
                ewgfState.ewgf = true;
            else
                ewgfState.wgf = true;
            ewgfState.state = 0;
            break;
    }
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer){
    SDL_Surface* surface = IMG_Load(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

DirectionalTextures loadTextures(SDL_Renderer* renderer){
    DirectionalTextures textures;
    
    textures.u = loadTexture("tekken/u.png", renderer);
    textures.d = loadTexture("tekken/d.png", renderer);
    textures.f = loadTexture("tekken/f.png", renderer);
    textures.b = loadTexture("tekken/b.png", renderer);
    textures.ub = loadTexture("tekken/ub.png", renderer);
    textures.uf = loadTexture("tekken/uf.png", renderer);
    textures.df = loadTexture("tekken/df.png", renderer);
    textures.db = loadTexture("tekken/db.png", renderer);
    textures.two = loadTexture("tekken/2.png", renderer);

    return textures;
}

int getDirection(const Uint8* keys){
    bool u = keys[SDL_SCANCODE_W];      // The tekken directional numpad
    bool b = keys[SDL_SCANCODE_A];      //          7 8 9
    bool d = keys[SDL_SCANCODE_S];      //          4 5 6
    bool f = keys[SDL_SCANCODE_D];      //          1 2 3

    // Need to check diagonals first so that they dont get skipped right after reaching cardinal input
    if(d && b) return 1;
    if(d && f) return 3;
    if(u && b) return 7;
    if(u && f) return 9;

    // Neutral
    if(!u && !d && !b && !f) return 5;

    // Cardinal directions
    if(d) return 2;
    if(b) return 4;
    if(f) return 6;
    if(u) return 8;

    return 5;
}

int getFace(const Uint8* keys){
    bool two = keys[SDL_SCANCODE_I];

    if(two) return 2;

    return 0;
}

int main() {
    SDL_Init(SDL_INIT_AUDIO);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cout << "Mix_OpenAudio error: " << Mix_GetError() << "\n";
    }

    Mix_Chunk* da = Mix_LoadWAV("sounds/jin_ewgf.mp3");
    Mix_Chunk* dorya = Mix_LoadWAV("sounds/dorya.mp3");

    SDL_Window* window = SDL_CreateWindow(
        "ewgf sim",               
        SDL_WINDOWPOS_CENTERED,      
        SDL_WINDOWPOS_CENTERED,      
        800, 600,                    
        SDL_WINDOW_SHOWN            
    );

    

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    DirectionalTextures textures = loadTextures(renderer);
    
    SDL_Event event;
    bool running = true;
    
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    const float targetFrameTime = 1000.0f/60.0f;
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last  = 0;
    double deltaTime = 0;

    while (running) {
        last = now;
        now = SDL_GetPerformanceCounter();

        deltaTime = (double)((now - last)*1000 / SDL_GetPerformanceFrequency());

        SDL_PumpEvents();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect r;
        r.w = 200;
        r.h = 200;
        r.x = (800 - r.w) / 2;
        r.y = (600 - r.h) / 2;

        int direction = getDirection(keyboardState);

        switch(direction){ // Switch case for each direction
            case 1: SDL_RenderCopy(renderer, textures.db, NULL, &r); break;
            case 2: SDL_RenderCopy(renderer, textures.d, NULL, &r); break;
            case 3: SDL_RenderCopy(renderer, textures.df, NULL, &r); break;
            case 4: SDL_RenderCopy(renderer, textures.b, NULL, &r);break;
            case 6: SDL_RenderCopy(renderer, textures.f, NULL, &r); break;
            case 7: SDL_RenderCopy(renderer, textures.ub, NULL, &r); break;
            case 8: SDL_RenderCopy(renderer, textures.u, NULL, &r); break;
            case 9: SDL_RenderCopy(renderer, textures.uf, NULL, &r); break;
        }

        updateEWGF(direction, getFace(keyboardState));
        if(ewgfState.ewgf){
            cout << "EWGF" << endl;
            Mix_PlayChannel(-1, da, 0);
        }
        else if(ewgfState.wgf){
            cout << "WGF" << endl;
            Mix_PlayChannel(-1, dorya, 0);
        }
        
        SDL_RenderPresent(renderer);
        double frameTime = (double)((SDL_GetPerformanceCounter() - now) * 1000 / SDL_GetPerformanceFrequency());
        if(frameTime < targetFrameTime) SDL_Delay(targetFrameTime - frameTime);

    }

    Mix_FreeChunk(da);    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
