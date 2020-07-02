#include <stdbool.h>
#include <SDL2/SDL.h>
#include <emscripten.h>

enum {
    DISPLAY_WIDTH  = 480
    , DISPLAY_HEIGHT = 320
    , RECT_SPEED = 2
};

class Sprite {
public:
    int x, y ;
    Sprite() :x(0), y(0) {}
} ;


const unsigned int size = 20;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Rect sprite = {0, 0, size, size};
Sprite point;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, &window, &renderer);
    if (window == NULL | renderer == NULL) {
        return false;
    } 

    return true;
}


void get_event(SDL_Event *event) {
    SDL_Keycode key = event->key.keysym.sym;
    
    if (event->key.type == SDL_KEYDOWN) {
        if (key == SDLK_LEFT || key == SDLK_RIGHT) {
            point.x = key == SDLK_LEFT ? -RECT_SPEED : RECT_SPEED;
        }
        if (key == SDLK_UP || key == SDLK_DOWN) {
            point.y = key == SDLK_UP ? -RECT_SPEED : RECT_SPEED;
        }
    }
    if (event->key.type == SDL_KEYUP) {
        point.x = 0;
        point.y = 0;
    }
}

void get_input() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        get_event(&event);
    }
}

void update() {
    sprite.x += point.x;
    sprite.y += point.y;
}

void draw() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &sprite);
    
    SDL_RenderPresent(renderer);
}

void main_loop() {

    get_input();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    update();
    draw();    

    SDL_RenderPresent(renderer);
    
}

void destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main() {
    init();
    emscripten_set_main_loop(main_loop, -1, 1);
    destroy();
    return EXIT_SUCCESS;
}