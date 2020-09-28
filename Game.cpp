#include "Game.hpp"

/*

-- small dimensions --
width: 640
height: 480

-- medium dimensions --
width: 840
height: 680

-- large dimensions --
width: 1040
height: 880

*/

const int grid_width = 640; //640
const int grid_height = 480; //480

const int grid_gap = 10;

const unsigned int gameSpeed = 100000; // In microseconds
// const unsigned int gameSpeed = 100000; // In microseconds

bool start = false;

bool leftMouseDown = false;
bool rightMouseDown = false;

struct cell {
    int limit_x;
    int limit_y;
    int x; // X position in grid
    int y; // Y position in grid
    bool active;
};

cell grid[grid_width/grid_gap][grid_height/grid_gap];

Game::Game() {
    //Fill grid array with square structs
    for (int x = 0; x < grid_width/grid_gap; x++)
        for (int y = 0; y < grid_height/grid_gap; y++)
            grid[x][y] = (cell){ (x+1)*grid_gap , (y+1)*grid_gap, x, y, false };
    
    std::cout << "\n--------------------------- " << std::endl;
    std::cout << "- Create cells by left clicking or holding left click and dragging. (Game must be stopped first)" << std::endl;
    std::cout << "- Delete cells by the same way but right clicking." << std::endl;
    std::cout << "- Clear grid by pressing backspace." << std::endl;
    std::cout << "- Press space to start/stop...\n" << std::endl;
}

Game::~Game() {
    std::cout << "\n\n-- Exiting." << std::endl;
    std::cout << "\n--------------------------- " << std::endl;
}

//initialize SDL
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

    //Everything has been setup successfully by this point
    isRunning = true;
    return true;
}

void Game::premade(int layout){
    if (layout == 1){
        for (int x = 0; x < grid_width/grid_gap; x++)
            for (int y = 0; y < grid_height/grid_gap; y++)
                if(x == 0 || y == 0 || x == grid_width/grid_gap-1 || y == grid_height/grid_gap-1)
                    grid[x][y].active = true;
    }else if (layout == 2){
        for (int x = 0; x < grid_width/grid_gap; x++)
            for (int y = 0; y < grid_height/grid_gap; y++)
                if(x == (grid_width/grid_gap-1)/2)
                    grid[x][y].active = true;
    }
}

// Sets all cell's active property to false
void Game::clearGrid(){
    for (int x = 0; x < grid_width/grid_gap; x++)
        for (int y = 0; y < grid_height/grid_gap; y++)
            grid[x][y].active = false;
}

//Apply rules to each cell
void Game::tick(){

    // *** RULES NEED TO APPLIED SIMULTANEOUSLY SO CHANGES ARE MADE TO THE COPY, THEN COPIED BACK TO THE ORIGINAL ***

    cell grid_copy[grid_width/grid_gap][grid_height/grid_gap];
    //Create copy of current grid
    for (int x = 0; x < grid_width/grid_gap; x++)
        for (int y = 0; y < grid_height/grid_gap; y++)
            grid_copy[x][y] = grid[x][y];

    int aliveNeighbors = 0;
    for (int x = 0; x < grid_width/grid_gap; x++){
        for (int y = 0; y < grid_height/grid_gap; y++){

            //Check if top cell exists
            if(y > 0 && grid[x][y-1].active) aliveNeighbors++; // Check if top is alive
            //Check if top right cell exists
            if((x < grid_width/grid_gap - 1 || y > 0) && grid[x+1][y-1].active) aliveNeighbors++; // Check if top right cell is alive
            //Check if right cell exists
            if((x < grid_width/grid_gap - 1) && grid[x+1][y].active) aliveNeighbors++; // Check if right is alive
            //Check if bottom right cell exists
            if((y < grid_height/grid_gap - 1 || x < grid_width/grid_gap - 1) && grid[x+1][y+1].active) aliveNeighbors++; // Check if bottom right is alive
            //Check if bottom cell exists
            if((y < grid_height/grid_gap - 1) && grid[x][y+1].active) aliveNeighbors++; // Check if bottom is alive
            //Check if bottom left cell exists
            if((x > 0 || y < grid_height/grid_gap - 1) && grid[x-1][y+1].active) aliveNeighbors++; // Check if bottom left is alive
            //Check if left cell exists
            if(x > 0 && grid[x-1][y].active) aliveNeighbors++; // Check if left is alive
            //Check if top left cell exists
            if((x > 0 || y > 0) && grid[x-1][y-1].active) aliveNeighbors++; // Check if top left is alive
            
            //Current cell is alive
            if(grid[x][y].active){
                //Less than or 2 or more than 3 die
                if(aliveNeighbors < 2 || aliveNeighbors > 3){
                    grid_copy[x][y].active = false;
                }
            }
            //Current cell is not alive and has exactly 3 neighbors
            else if(aliveNeighbors == 3){
                grid_copy[x][y].active = true;
            }
            aliveNeighbors = 0; //Reset neighbor count
        }
    }

    //Put contents into original array
    for (int x = 0; x < grid_width/grid_gap; x++)
        for (int y = 0; y < grid_height/grid_gap; y++)
            grid[x][y] = grid_copy[x][y];
}


void Game::handleEvents() {
    SDL_Event e;
    SDL_PollEvent(&e);
    switch (e.type){
        case SDL_QUIT:
            isRunning = false;
            break;
        // Left/right mouse button down
        case SDL_MOUSEBUTTONDOWN:
                if(e.button.button == SDL_BUTTON_LEFT) handleClick(SDL_BUTTON_LEFT);
                else handleClick(SDL_BUTTON_RIGHT);
            break;
        // Left/right mouse button up
        case SDL_MOUSEBUTTONUP:
            leftMouseDown = false; //Disable mouse drag
            rightMouseDown = false; //Disable mouse drag
            break;
        case SDL_MOUSEMOTION:
            if (leftMouseDown) handleClick(SDL_BUTTON_LEFT); //Allow mouse drag draw
            if (rightMouseDown) handleClick(SDL_BUTTON_RIGHT); //Allow mouse drag draw
            break;
        // Keyboard event
        case SDL_KEYDOWN:
            // Determine key pressed
            switch (e.key.keysym.sym){
                case SDLK_SPACE:
                    start = !start; //Toggle game
                    if (start) std::cout << "-- Game started" << std::endl;
                    else std::cout << "-- Game stopped" << std::endl;
                    break;
                case SDLK_BACKSPACE:
                    clearGrid(); //Clear grid
                    std::cout << "-- Grid cleared" << std::endl;
                    break;
                case SDLK_q:
                    premade(1);
                    break;
                case SDLK_w:
                    premade(2);
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

//Left click places the cell, right click deletes a placed cell
void Game::handleClick(int key){
    if(start) return;

    if(key == SDL_BUTTON_LEFT) leftMouseDown = true;
    else rightMouseDown = true;

    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse); //Get current mouse coordinates
    
    //repeat for number of rows and columns
    for (int x = 0; x < grid_width/grid_gap; x++){
        for (int y = 0; y < grid_height/grid_gap; y++){
            //Check if clicked within limits of cell            
            if(xMouse < grid[x][y].limit_x && xMouse > grid[x][y].limit_x - grid_gap){
                if(yMouse < grid[x][y].limit_y && yMouse > grid[x][y].limit_y - grid_gap){
                    if(key == SDL_BUTTON_LEFT) grid[x][y].active = true;
                    else grid[x][y].active = false;
                }
            }
        }
    }
}

//Render loop
void Game::render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    DrawGrid(renderer);
    if(start){
        tick();
        usleep(gameSpeed);
    }
}

// Update game grid
void Game::DrawGrid(SDL_Renderer* renderer){
    
    SDL_SetRenderDrawColor(renderer, 35, 35, 35, SDL_ALPHA_OPAQUE);

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

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    
    //Update for clicked alive cells
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