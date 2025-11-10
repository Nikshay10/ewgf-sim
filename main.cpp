#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Hello SDL2!",               
        SDL_WINDOWPOS_CENTERED,      
        SDL_WINDOWPOS_CENTERED,      
        800, 600,                    
        SDL_WINDOW_SHOWN            
    );

    if (!window) {
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool running = true;

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
                running = false;

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE)
                    cout << "Spacebar pressed!\n";
            }
        }
        SDL_Delay(16); // ~60 FPS delay
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
