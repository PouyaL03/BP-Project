#include "helping-functions.h"
#define window_width 640
#define window_height 480
const int FPS=24;

/*function prototypes*/
void draw_map();

void draw_map()
{
    /*initializing graphic system*/
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0)
    {
        printf("error initializing is: %s\n", SDL_GetError());
        return;
    }
    /*initializinig the windows*/
    SDL_Window* window= SDL_CreateWindow("Initial window",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        window_width, window_height, 
                                        SDL_WINDOW_RESIZABLE);
    if (window==NULL)
    {
        printf("could not create window: %s\n", SDL_GetError());
        return;
    }
    /*creating the renderer*/
    Uint32 render_flags=SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
    if (renderer==NULL)
    {
      printf("error creating renderer: %s\n", SDL_GetError());
      SDL_DestroyWindow(window);
      SDL_Quit();
      return;
    }
    /*select color of background*/
    SDL_Surface* surface = IMG_Load("../images/sample-picture.png");
    if (surface==NULL)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    /*loading the texture to the windows*/
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture==NULL)
    {
    // clean up resources before exiting
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    /*creating a single rectangle to save the pic into it*/
    SDL_Rect destination;
    SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
    destination.w/=4;
    destination.h/=4;
    float x_position=(window_width-destination.w)/2;
    float y_position=(window_height-destination.h)/2;
    /*setting background color*/
    SDL_SetRenderDrawColor(renderer, 255, 99, 71, 255);
    destination.x=x_position;
    destination.y=y_position;
    int close_requested=0;
    while (!close_requested)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                printf("khaste nabashid.\n");
                close_requested=1;
                break;
            }
        }
        // SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 99, 71, 255);
        SDL_RenderCopy(renderer, texture, NULL, &destination);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }
    // clean up resources before exiting
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
}