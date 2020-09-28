#include "Game.hpp"
#include <string>


int main(){
    Game game;
    bool successful = game.init("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480);

    if(successful){
        while (game.running()){
            game.handleEvents();
            game.render();
        }
        game.clean();
    }
    return 0;
}
