#include "Game.hpp"
#include <iostream>
#include <unistd.h>

const unsigned int microseconds = 150000; //250000

const int grid_width = 940;
const int grid_height = 780;

// const int grid_width = 640;
// const int grid_height = 480;

const int grid_gap = 20;

int generation = 0;

bool start = false;

struct sqaure {
    int limit_x;
    int limit_y;
    int x; // X position in grid
    int y; // Y position in grid
    bool active;
};

sqaure grid[grid_width/grid_gap][grid_height/grid_gap];

Game::Game() {}

Game::~Game() {}

bool Game::init(const char *title, int xpos, int ypos, int width, int height) {

    // Initialize SDL2
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP;

    // Create an application window with the following settings:
    window = SDL_CreateWindow(title, xpos, ypos, width, height, window_flags);
    if(window == nullptr){
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr){
        printf( "Could not create renderer! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    //Fill grid array with square structs
    for (int x = 0; x < grid_width/grid_gap; x++)
        for (int y = 0; y < grid_height/grid_gap; y++)
            grid[x][y] = (sqaure){ (x+1)*grid_gap , (y+1)*grid_gap, x, y, false };

    //Everything has been setup successfully by this point
    isRunning = true;
    std::cout << "Press space to start" << std::endl;
    return true;
}

void Game::handleEvents() {
    SDL_Event e;
    SDL_PollEvent(&e);
    switch (e.type){
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleClick();
            break;
        case SDL_KEYDOWN:
            if(e.type == 768) start = true;
            break;
        default:
            break;
    }
}


/*
        (x-1, y+1) (x, y+1 ) (x+1, y+1 )
             [ * ] [ * ] [ * ]
    (x-1, y) [ * ] [ 0 ] [ * ] (x+1, y)
             [ * ] [ * ] [ * ] 
        (x-1, y-1) (x, y-1 ) (x+1, y-1 )
*/
void tick(){

    // *** RULES NEED TO APPLIED SIMULTANEOUSLY SO CHANGES ARE MADE TO THE COPY, THEN COPIED BACK TO THE ORIGINAL ***

    sqaure grid_copy[grid_width/grid_gap][grid_height/grid_gap];
    //Create copy of current grid
    for (int x = 0; x < grid_width/grid_gap; x++)
        for (int y = 0; y < grid_height/grid_gap; y++)
            grid_copy[x][y] = grid[x][y];

    int aliveNeighbors = 0;
    for (int x = 0; x < grid_width/grid_gap; x++){
        for (int y = 0; y < grid_height/grid_gap; y++){

            //Get number of alive aliveNeighbors

            if(grid[x-1][y+1].active) aliveNeighbors++; // Top left
            if(grid[x][y+1].active) aliveNeighbors++;   // Top
            if(grid[x+1][y+1].active) aliveNeighbors++; // Top right

            if(grid[x+1][y].active) aliveNeighbors++;   // Right
            if(grid[x-1][y].active) aliveNeighbors++;   // Left

            if(grid[x+1][y-1].active) aliveNeighbors++; // Bottom right
            if(grid[x][y-1].active) aliveNeighbors++;   // Bottom
            if(grid[x-1][y-1].active) aliveNeighbors++; // Bottom left

            if(grid[x][y].active){
                if(aliveNeighbors < 2 || aliveNeighbors > 3) grid_copy[x][y].active = false;
            }
            else if(aliveNeighbors == 3) grid_copy[x][y].active = true;
            aliveNeighbors = 0;
        }
    }
    //Put contents into original array
    for (int x = 0; x < grid_width/grid_gap; x++)
        for (int y = 0; y < grid_height/grid_gap; y++)
            grid[x][y] = grid_copy[x][y];
    
    std::cout << "Generation: " << generation << std::endl;
    generation++;
}

void Game::handleClick(){
    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse);

    if(start) return;
    

    //repeat for number of rows and columns
    for (int x = 0; x < grid_width/grid_gap; x++){
        for (int y = 0; y < grid_height/grid_gap; y++){
            //Check if clicked within limits of cell            
            if(xMouse < grid[x][y].limit_x && xMouse > grid[x][y].limit_x - grid_gap){
                if(yMouse < grid[x][y].limit_y && yMouse > grid[x][y].limit_y - grid_gap){
                    grid[x][y].active = true;
                }
            }
        }
    }
}

void Game::update(){}

void Game::render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    DrawGrid(renderer);
    if(start){
        tick();
        usleep(microseconds);
    }
    
}

void Game::DrawGrid(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    //Draw vertical lines
    for (int x = 1; x*grid_gap <= grid_width; x++){
        SDL_RenderDrawLine(renderer,
            x*grid_gap, 0,
            x*grid_gap, grid_height
        );
    }
    //Draw horizontal lines
    for (int y = 1; y*grid_gap <= grid_height; y++){
        SDL_RenderDrawLine(renderer,
            0, y*grid_gap,
            grid_width, y*grid_gap
        );
    }
    
    //Update for clicked cells
    for (int x = 0; x < grid_width/grid_gap; x++){
        for (int y = 0; y < grid_height/grid_gap; y++){
            if(grid[x][y].active){
                SDL_Rect rect = {
                    grid[x][y].limit_x - grid_gap, //location x
                    grid[x][y].limit_y - grid_gap, //location y
                    grid_gap, grid_gap //h, w
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}


