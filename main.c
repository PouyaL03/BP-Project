#include "game.h"
/*function prototypes*/
void menu(SDL_Renderer*, TTF_Font*, TTF_Font*);

int main()
{
    /*initializing graphic system*/
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    /*initializinig the windows*/
    Uint32 window_flags=SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* window= SDL_CreateWindow("Initial window",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        window_width, window_height, 
                                        window_flags);
    /*creating the renderer*/
    Uint32 render_flags=SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
    /*opening the font*/
    TTF_Init();
    TTF_Font* font=TTF_OpenFont("../fonts/TNR.ttf", 26);
    TTF_Font* bold_font=TTF_OpenFont("../fonts/TNR_B.ttf", 24);
    /*initializing countries*/
    country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row];
    create_random_map(all_countries);
    country country_array[number_of_countries];
    initialize_country_array(country_array, all_countries);
    // menu(renderer, font, bold_font);
    start_game(renderer, font, bold_font, country_array);
    // clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void menu(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* bold_font)
{
    int waiting_for_client=1;
    while(waiting_for_client)
    {

    }
}