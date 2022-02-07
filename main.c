#include "menu.h"
/*function prototypes*/
void main_menu(SDL_Renderer*, TTF_Font*, TTF_Font*);

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
    main_menu(renderer, font, bold_font);
    start_game(renderer, font, bold_font, country_array);
    // clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_CloseFont(bold_font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void main_menu(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* bold_font)
{
    button_width=window_width/3;
    SDL_Event event;
    SDL_Point mouse_position;
    /*initializing window background and buttons*/
    SDL_Surface* background_surface=IMG_Load("../images/background.webp");
    SDL_Texture* background_texture=SDL_CreateTextureFromSurface(renderer, background_surface);
    SDL_Surface* button_surface=IMG_Load("../images/button.png");
    SDL_Texture* button_texture=SDL_CreateTextureFromSurface(renderer, button_surface);
    TTF_Font* state_io_text_font=TTF_OpenFont("../fonts/TNR_B.ttf", 120);
    SDL_Color state_io_textColor={255, 0, 0, 255};
    SDL_Surface* surface_state_io=TTF_RenderText_Solid(state_io_text_font, "state.io", state_io_textColor);
    SDL_Texture* texture_state_io=SDL_CreateTextureFromSurface(renderer, surface_state_io);
    TTF_Font* button_text_font=TTF_OpenFont("../fonts/TNR_B.ttf", 40);
    SDL_Color button_textColor={255, 255, 255, 255};
    SDL_Surface* new_game_text_surface=TTF_RenderText_Solid(button_text_font, "NEW GAME", button_textColor);
    SDL_Texture* new_game_text_texture=SDL_CreateTextureFromSurface(renderer, new_game_text_surface);
    SDL_Surface* resume_game_text_surface=TTF_RenderText_Solid(button_text_font, "RESUME GAME", button_textColor);
    SDL_Texture* resume_game_text_texture=SDL_CreateTextureFromSurface(renderer, resume_game_text_surface);
    SDL_Surface* scoreboard_text_surface=TTF_RenderText_Solid(button_text_font, "SCOREBOARD", button_textColor);
    SDL_Texture* scoreboard_text_texture=SDL_CreateTextureFromSurface(renderer, scoreboard_text_surface);
    TTF_Font* description_font=TTF_OpenFont("../fonts/TNR.ttf", 30);
    SDL_Color description_textColor={0, 0, 0, 255};
    SDL_Surface* description_text_surface=TTF_RenderText_Solid(description_font, "Developed by:Pouya Lahabi", description_textColor);
    SDL_Texture* description_text_texture=SDL_CreateTextureFromSurface(renderer, description_text_surface);
    /*setting rectangles*/
    SDL_Rect background_rect;
    background_rect.h=window_height;
    background_rect.w=window_width;
    background_rect.x=0;
    background_rect.y=0;
    SDL_Rect new_game_button_rect;
    new_game_button_rect.h=button_height;
    new_game_button_rect.w=button_width;
    new_game_button_rect.x=(window_width-button_width)/2;
    new_game_button_rect.y=400;
    SDL_Rect resume_game_button_rect;
    resume_game_button_rect.h=button_height;
    resume_game_button_rect.w=button_width;
    resume_game_button_rect.x=(window_width-button_width)/2;
    resume_game_button_rect.y=500;
    SDL_Rect scoreboard_button_rect;
    scoreboard_button_rect.h=button_height;
    scoreboard_button_rect.w=button_width;
    scoreboard_button_rect.x=(window_width-button_width)/2;
    scoreboard_button_rect.y=600;
    SDL_Rect state_io_text_rect;
    state_io_text_rect.x = (window_width-surface_state_io->w)/2;
    state_io_text_rect.y = 200;
    state_io_text_rect.w = surface_state_io->w;
    state_io_text_rect.h = surface_state_io->h;
    SDL_Rect new_game_text_rect;
    new_game_text_rect.x=new_game_button_rect.x+(new_game_button_rect.w-new_game_text_surface->w)/2;
    new_game_text_rect.y=new_game_button_rect.y+(new_game_button_rect.h-new_game_text_surface->h)/2;
    new_game_text_rect.w=new_game_text_surface->w;
    new_game_text_rect.h=new_game_text_surface->h;
    SDL_Rect resume_game_text_rect;
    resume_game_text_rect.x=resume_game_button_rect.x+(resume_game_button_rect.w-resume_game_text_surface->w)/2;
    resume_game_text_rect.y=resume_game_button_rect.y+(resume_game_button_rect.h-resume_game_text_surface->h)/2;
    resume_game_text_rect.w=resume_game_text_surface->w;
    resume_game_text_rect.h=resume_game_text_surface->h;
    SDL_Rect scoreboard_text_rect;
    scoreboard_text_rect.x=scoreboard_button_rect.x+(scoreboard_button_rect.w-scoreboard_text_surface->w)/2;
    scoreboard_text_rect.y=scoreboard_button_rect.y+(scoreboard_button_rect.h-scoreboard_text_surface->h)/2;
    scoreboard_text_rect.w=scoreboard_text_surface->w;
    scoreboard_text_rect.h=scoreboard_text_surface->h;
    SDL_Rect description_text_rect;
    description_text_rect.x=(window_width-description_text_surface->w)/2;
    description_text_rect.y=800;
    description_text_rect.w=description_text_surface->w;
    description_text_rect.h=description_text_surface->h;
    /*freeing surfaces and fonts*/
    SDL_FreeSurface(button_surface);
    SDL_FreeSurface(background_surface);
    SDL_FreeSurface(surface_state_io);
    SDL_FreeSurface(new_game_text_surface);
    SDL_FreeSurface(resume_game_text_surface);
    SDL_FreeSurface(scoreboard_text_surface);
    SDL_FreeSurface(description_text_surface);
    TTF_CloseFont(state_io_text_font);
    TTF_CloseFont(button_text_font);
    TTF_CloseFont(description_font);
    while(1)
    {
        while(SDL_PollEvent(&event))
        {
            SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
            switch(event.type)
            {
                case (SDL_QUIT):
                    SDL_DestroyTexture(background_texture);
                    SDL_DestroyTexture(button_texture);
                    SDL_DestroyTexture(texture_state_io);
                    SDL_DestroyTexture(new_game_text_texture);
                    SDL_DestroyTexture(resume_game_text_texture);
                    SDL_DestroyTexture(scoreboard_text_texture);
                    SDL_DestroyTexture(description_text_texture);
                    return;
                case (SDL_MOUSEBUTTONDOWN):
                    if (mouse_position.x > new_game_button_rect.x && mouse_position.x < new_game_button_rect.x + new_game_button_rect.w &&
                        mouse_position.y > new_game_button_rect.y && mouse_position.y < new_game_button_rect.y + new_game_button_rect.h)
                        {
                            printf("trying to open new game menu.\n");
                            SDL_RenderClear(renderer);
                            new_game_menu(renderer, background_texture, button_texture, font, bold_font, background_rect);
                        }
                    else if (mouse_position.x > resume_game_button_rect.x && mouse_position.x < resume_game_button_rect.x + resume_game_button_rect.w &&
                             mouse_position.y > resume_game_button_rect.y && mouse_position.y < resume_game_button_rect.y + resume_game_button_rect.h)
                        {
                            printf("trying to resume game for you.\n");
                            SDL_RenderClear(renderer);
                            resume_game(renderer, background_texture, button_texture, font, bold_font, background_rect);
                        }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);
        SDL_RenderCopy(renderer, button_texture, NULL, &new_game_button_rect);
        SDL_RenderCopy(renderer, button_texture, NULL, &resume_game_button_rect);
        SDL_RenderCopy(renderer, button_texture, NULL, &scoreboard_button_rect);
        SDL_RenderCopy(renderer, texture_state_io, NULL, &state_io_text_rect);
        SDL_RenderCopy(renderer, new_game_text_texture, NULL, &new_game_text_rect);
        SDL_RenderCopy(renderer, resume_game_text_texture, NULL, &resume_game_text_rect);
        SDL_RenderCopy(renderer, scoreboard_text_texture, NULL, &scoreboard_text_rect);
        SDL_RenderCopy(renderer, description_text_texture, NULL, &description_text_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(button_texture);
    SDL_DestroyTexture(texture_state_io);
    SDL_DestroyTexture(new_game_text_texture);
    SDL_DestroyTexture(resume_game_text_texture);
    SDL_DestroyTexture(scoreboard_text_texture);
    SDL_DestroyTexture(description_text_texture);
}
/*
    SDL_Rect rectangle;
    SDL_Texture* texture;
    SDL_Color textColor={0, 0, 0, 255};
    SDL_Surface* surface;
    if (waiting_for_attack==0) surface=TTF_RenderText_Solid(font, "please choose country to attack from", textColor);
    else if (waiting_for_attack==1) surface=TTF_RenderText_Solid(font, "plase choose where you want to attack", textColor);
    texture=SDL_CreateTextureFromSurface(renderer, surface);
    int text_width = surface->w;
    int text_height = surface->h;
    SDL_FreeSurface(surface);
    rectangle.x = (window_width-text_width)/2;
    rectangle.y = window_height-(text_height+10);
    rectangle.w = text_width;
    rectangle.h = text_height;
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
    SDL_DestroyTexture(texture);*/