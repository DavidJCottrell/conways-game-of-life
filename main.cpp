#include "Game.hpp"
#include <string>


int main(){

    Game game;
    game.init("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 940, 780);

    while (game.running()){
        game.handleEvents();
        game.update();
        game.render();
    }

    game.clean();

    return 0;
}
