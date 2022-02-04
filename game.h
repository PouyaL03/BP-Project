#include "map.h"

/*function prototypes*/
void start_game();

void start_game()
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
    int total_frames=0;
    /*initializing countries*/
    country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row];
    create_random_map(all_countries);
    country country_array[number_of_countries];
    initialize_country_array(country_array, all_countries);
    attack* attack_head=NULL;
    /*------------------------------------------------------------------------------------*/
    while (!close_requested)
    {
        SDL_Event event;
        event_handling(&event, country_array, mouse_position, &close_requested, &attack_head);
        update_number_of_soldiers(country_array, total_frames);
        SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
        check_mouse_state(mouse_position, country_array, initial_side_length*sin(teta));
        SDL_SetRenderDrawColor(renderer,
                               colors[background_color].r,
                               colors[background_color].g,
                               colors[background_color].b, 255);
        SDL_RenderClear(renderer);
        update_attacking_soldiers_position(country_array, &attack_head, total_frames);
        draw_map(renderer, country_array, bold_font, font, &attack_head);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
        total_frames++;
    }
    // clean up resources before exiting
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
}