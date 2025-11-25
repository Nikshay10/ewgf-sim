#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "ewgf sim",               
        SDL_WINDOWPOS_CENTERED,      
        SDL_WINDOWPOS_CENTERED,      
        800, 600,                    
        SDL_WINDOW_SHOWN            
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* up =  IMG_Load("tekken/u.png");
    SDL_Surface* back = IMG_Load("tekken/b.png");
    SDL_Surface* front = IMG_Load("tekken/f.png");
    SDL_Surface* down = IMG_Load("tekken/d.png");
    SDL_Surface* Two = IMG_Load("tekken/2.png");
  

    SDL_Texture* u = SDL_CreateTextureFromSurface(renderer, up);
    SDL_Texture* d = SDL_CreateTextureFromSurface(renderer, down);
    SDL_Texture* f = SDL_CreateTextureFromSurface(renderer, front);
    SDL_Texture* b = SDL_CreateTextureFromSurface(renderer, back);
    SDL_Texture* two = SDL_CreateTextureFromSurface(renderer, Two);
    SDL_FreeSurface(up);
    SDL_FreeSurface(down);
    SDL_FreeSurface(front);
    SDL_FreeSurface(back);
    SDL_FreeSurface(Two);

    vector<SDL_Texture*> textures = {u, d, b, f, two};

    SDL_Event event;
    bool running = true;
    bool showImage = false;

    map<int, bool> keyMap = {{119,false},
                                {97,false},
                                {115,false},
                                {100,false},
                                {105,false},};

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
                running = false;

            if (event.type == SDL_KEYDOWN) {
                keyMap[event.key.keysym.sym] = true;
                cout << event.key.keysym.sym << endl;
            }

            if (event.type == SDL_KEYUP) {
                keyMap[event.key.keysym.sym] = false;
                cout << event.key.keysym.sym << endl;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (keyMap[119]) {
            SDL_Rect destRect = { 200, 150, 400, 300 }; 
            SDL_RenderCopy(renderer, u, NULL, &destRect);
        }
        if (keyMap[97]) {
            SDL_Rect destRect = { 200, 150, 400, 300 }; 
            SDL_RenderCopy(renderer, b, NULL, &destRect);
        }
        if (keyMap[115]) {
            SDL_Rect destRect = { 200, 150, 400, 300 }; 
            SDL_RenderCopy(renderer, d, NULL, &destRect);
        }
        if (keyMap[100]) {
            SDL_Rect destRect = { 200, 150, 400, 300 }; 
            SDL_RenderCopy(renderer, f, NULL, &destRect);
        }
        if (keyMap[105]) {
            SDL_Rect destRect = { 200, 150, 400, 300 }; 
            SDL_RenderCopy(renderer, two, NULL, &destRect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS delay
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
