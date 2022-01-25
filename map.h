#include "helping-functions.h"

/*function prototypes*/
void start_game();
void draw_map(SDL_Renderer*, country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row]);

void draw_map(SDL_Renderer* renderer, country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row])
{
    for (int i=0 ; i<number_of_hexagons_in_column ; i++)
    {
        for (int j=0 ; j<number_of_hexagons_in_row ; j++)
        {
            Uint8 alpha=255;
            draw_hexagon_borderline(renderer, all_countries[i][j].x_center,
                                   all_countries[i][j].y_center,
                                   initial_side_length,
                                   all_countries[i][j].color,
                                   alpha);
        }
    }
}

void start_game()
{
    /*initializing graphic system*/
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    /*initializinig the windows*/
    Uint32 window_flags=SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* window= SDL_CreateWindow("Initial window",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        window_width, window_height, 
                                        window_flags);
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
    SDL_SetRenderDrawColor(renderer, colors[7].r, colors[7].g, colors[7].b, 255);
    SDL_Point mouse_position;
    int close_requested=0;
    country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row];
    create_random_map(all_countries);
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
            }
        }
        SDL_SetRenderDrawColor(renderer, colors[7].r, colors[7].g, colors[7].b, 255);
        SDL_RenderClear(renderer);
        draw_map(renderer, all_countries);
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