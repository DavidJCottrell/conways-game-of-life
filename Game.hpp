#ifndef Game_hpp
#define Game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>

#include <iostream>

class Game {
   public:
    Game();
    ~Game();

    bool init(const char* title, int xpos, int ypos, int width, int height);
    void handleEvents();
    void render();
    void clean();  // memory management

    bool running() { return isRunning; }

   private:
    bool isRunning;

    void DrawGrid(SDL_Renderer* renderer);
    void clearGrid();
    void premade(int layout);  // Draw premade configurations
    void tick();

    void handleClick(int key);

    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif /* Game_hpp */
