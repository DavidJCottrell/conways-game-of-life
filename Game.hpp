#ifndef Game_hpp
#define Game_hpp
 
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

class Game {
public:

    Game();
    ~Game();

    bool init(const char* title, int xpos, int ypos, int width, int height);
    void handleEvents();
    void render();
    void clean(); //memory management

    void DrawGrid(SDL_Renderer* renderer);
    void clearGrid();
    void tick();

    void handleClick(int key);

    bool running() { return isRunning; }

private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;

    
};



#endif /* Game_hpp */
