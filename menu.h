#include "game.h"
/*function prototypes*/
void new_game_menu(SDL_Renderer*, SDL_Texture*, SDL_Texture*, TTF_Font*, TTF_Font*, SDL_Rect);
void resume_game(SDL_Renderer*, SDL_Texture*, SDL_Texture*, TTF_Font*, TTF_Font*, SDL_Rect);

void new_game_menu(SDL_Renderer* renderer, SDL_Texture* background_texture, SDL_Texture* button_texture, TTF_Font* font, TTF_Font* bold_font, SDL_Rect background_rect)
{
    TTF_Font* tmp_font=TTF_OpenFont("../fonts/TNR_B.ttf", 180);
    SDL_Color tmp_textColor={255, 0, 0, 255};
    SDL_Surface* tmp_surface=TTF_RenderText_Solid(tmp_font, "Under Construction", tmp_textColor);
    SDL_Texture* tmp_texture=SDL_CreateTextureFromSurface(renderer, tmp_surface);
    SDL_Rect tmp_rect;
    tmp_rect.x=(window_width-tmp_surface->w)/2;
    tmp_rect.y=(window_height-tmp_surface->h)/2;
    tmp_rect.w=tmp_surface->w;
    tmp_rect.h=tmp_surface->h;
    for (int i=0 ; i<5000 ; i++)
    {
    SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);
    SDL_RenderCopy(renderer, tmp_texture, NULL, &tmp_rect);
    SDL_Delay(1);
    }
    TTF_CloseFont(tmp_font);
    SDL_DestroyTexture(tmp_texture);
    SDL_FreeSurface(tmp_surface);
    
}

void resume_game(SDL_Renderer* renderer, SDL_Texture* background_texture, SDL_Texture* button_texture, TTF_Font* font, TTF_Font* bold_font, SDL_Rect background_rect)
{
    TTF_Font* tmp_font=TTF_OpenFont("../fonts/TNR_B.ttf", 180);
    SDL_Color tmp_textColor={255, 0, 0, 255};
    SDL_Surface* tmp_surface=TTF_RenderText_Solid(tmp_font, "Under Construction", tmp_textColor);
    SDL_Texture* tmp_texture=SDL_CreateTextureFromSurface(renderer, tmp_surface);
    SDL_Rect tmp_rect;
    tmp_rect.x=(window_width-tmp_surface->w)/2;
    tmp_rect.y=(window_height-tmp_surface->h)/2;
    tmp_rect.w=tmp_surface->w;
    tmp_rect.h=tmp_surface->h;
    SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);
    SDL_RenderCopy(renderer, tmp_texture, NULL, &tmp_rect);
    SDL_Delay(5000);
    TTF_CloseFont(tmp_font);
    SDL_DestroyTexture(tmp_texture);
    SDL_FreeSurface(tmp_surface);
}
/*
    //initializing countires
    country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row];
    create_random_map(all_countries);
    country country_array[number_of_countries];
    initialize_country_array(country_array, all_countries);
    main_menu(renderer, font, bold_font);
    start_game(renderer, font, bold_font, country_array);    
*/