#include "helping-functions.h"

/*function prototypes*/
void draw_map();

void draw_map()
{
    /*initializing graphic system*/
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    /*initializinig the windows*/
    SDL_Window* window= SDL_CreateWindow("Initial window",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        window_width, window_height, 
                                        SDL_WINDOW_RESIZABLE);
    /*creating the renderer*/
    Uint32 render_flags=SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
    /*select color of background*/
    SDL_Surface* surface = IMG_Load("../images/sample-picture.png");
    /*loading the texture to the windows*/
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    /*creating a single rectangle to save the pic into it*/
    SDL_Rect destination;
    SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
    destination.w/=4;
    destination.h/=4;
    float x_position=(window_width-destination.w)/2;
    float y_position=(window_height-destination.h)/2;
    /*setting background color*/
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    destination.x=x_position;
    destination.y=y_position;
    SDL_Point mouse_position;
    int close_requested=0;
    /*------------------------------------------------------------------------------------*/
    while (!close_requested)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case (SDL_QUIT):
                    printf("khaste nabashid.\n");
                    close_requested=1;
                    break;
                case (SDL_WINDOWEVENT):
                    switch(event.window.event)
                    {
                        case(SDL_WINDOWEVENT_RESIZED):
                            window_width=event.window.data1;
                            window_height=event.window.data2;
                            printf("window resized.\n");
                            break;
                    }
            }
        }
        SDL_RenderClear(renderer);
        // SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        // SDL_Delay(1000);
        // SDL_RenderCopy(renderer, texture, NULL, &destination);
        draw_hexagon(renderer, window_width/2, window_height/2, 0);
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