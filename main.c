#include "menu.h"
/*function prototypes*/
void main_menu(SDL_Renderer*, TTF_Font*, TTF_Font*);
int get_username(SDL_Renderer*, SDL_Texture*, SDL_Rect);
void update_username_box(SDL_Renderer*, SDL_Surface**, SDL_Texture**, SDL_Rect*, char*, TTF_Font*, SDL_Color, SDL_Rect);
void SDL_DestroyEverything_main_menu(SDL_Texture**, SDL_Texture**, SDL_Texture**, SDL_Texture**, SDL_Texture**, SDL_Texture**, SDL_Texture**);
void find_user(char*);

int main()
{
    /*initializing graphic system*/
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    TTF_Init();
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
    TTF_Font* font=TTF_OpenFont("../fonts/TNR.ttf", 26);
    TTF_Font* bold_font=TTF_OpenFont("../fonts/TNR_B.ttf", 24);
    /*initializing countries*/
    // country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row];
    // create_random_map(all_countries);
    // country country_array[number_of_countries];
    // initialize_country_array(country_array, all_countries);
    main_menu(renderer, font, bold_font);
    // start_game(renderer, font, bold_font, country_array);
    // clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_CloseFont(bold_font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void update_username_box(SDL_Renderer* renderer, SDL_Surface** surface, SDL_Texture** texture, SDL_Rect* rect, char* line, TTF_Font* font, SDL_Color color, SDL_Rect relative_rect)
{
    *surface=TTF_RenderText_Solid(font, line, color);
    *texture=SDL_CreateTextureFromSurface(renderer, *surface);
    rect->h=(*surface)->h;
    rect->w=(*surface)->w;
    rect->x=window_width/2-190;
    rect->y=relative_rect.y+relative_rect.h+20;
}

int get_username(SDL_Renderer* renderer, SDL_Texture* background_texture, SDL_Rect background_rect)
{
    SDL_Event event;
    SDL_Point mouse_position;
    TTF_Font* enter_font=TTF_OpenFont("../fonts/TNR_B.ttf", 60);
    SDL_Color enter_textColor={255, 0, 0, 255};
    SDL_Surface* enter_surface=TTF_RenderText_Solid(enter_font, "Please Enter Your Username", enter_textColor);
    SDL_Texture* enter_texture=SDL_CreateTextureFromSurface(renderer, enter_surface);
    SDL_Rect enter_rect;
    enter_rect.x=(window_width-enter_surface->w)/2;
    enter_rect.y=200;
    enter_rect.w=enter_surface->w;
    enter_rect.h=enter_surface->h;
    TTF_Font* username_font=TTF_OpenFont("../fonts/TNR.ttf", 30);
    SDL_Color username_color={0, 0, 0, 255};
    SDL_Surface* username_text_surface;
    SDL_Texture* username_text_texture;
    SDL_Rect username_text_rect;
    TTF_Font* warning_font=TTF_OpenFont("../fonts/TNR.ttf", 40);
    SDL_Color warning_textColor={255, 0, 0, 255};
    SDL_Surface* warning_surface=TTF_RenderText_Solid(warning_font, "your username must have less than 20 characters", warning_textColor);
    SDL_Texture* warning_texture=SDL_CreateTextureFromSurface(renderer, warning_surface);
    SDL_Rect warning_rect;
    warning_rect.x=(window_width-warning_surface->w)/2;
    warning_rect.y=800;
    warning_rect.w=warning_surface->w;
    warning_rect.h=warning_surface->h;
    int show_warning=0;
    int return_type=nothing;
    int close_requested=0;
    int index=0;
    char temp_username[21];
    int max_username_length=20;
    for (int i=0 ; i<max_username_length+1 ; i++)
    {
        temp_username[i]='\0';
    }
    sprintf(temp_username, " ");
    update_username_box(renderer, &username_text_surface, &username_text_texture, &username_text_rect, temp_username, username_font, username_color, enter_rect);
    SDL_StartTextInput();

    while(!close_requested)
    {
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case (SDL_QUIT):
                    return_type=game_quit;
                    close_requested=1;
                    break;
                case (SDL_KEYDOWN):
                    switch(event.key.keysym.sym)
                    {
                        case (SDLK_BACKSPACE):
                            show_warning=0;
                            if (index>1)
                            {
                                temp_username[index-1]='\0';
                                index--;
                            }
                            else
                            {
                                if (index==1)
                                {
                                    temp_username[index-1]=' ';
                                    index--;
                                }
                            }
                            update_username_box(renderer, &username_text_surface, &username_text_texture, &username_text_rect, temp_username, username_font, username_color, enter_rect);
                            break;
                        case (SDLK_RETURN):
                            close_requested=1;
                            return_type=nothing;
                            find_user(temp_username);
                            return nothing;
                    }
                    break;
                case (SDL_TEXTINPUT):
                    if (strlen(temp_username)>=max_username_length)
                    {
                        show_warning=1;
                    }
                    else
                    {
                    strcpy(temp_username+index, event.text.text);
                    index=strlen(temp_username);
                    update_username_box(renderer, &username_text_surface, &username_text_texture, &username_text_rect, temp_username, username_font, username_color, enter_rect);
                    }
                    break;
            }
        }
        SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);
        SDL_RenderCopy(renderer, enter_texture, NULL, &enter_rect);
        boxRGBA(renderer, window_width/2-205, enter_rect.y+enter_rect.h+10, window_width/2+205,
                                              enter_rect.y+enter_rect.h+65, 0, 0, 0, 255);
        boxRGBA(renderer, window_width/2-200, enter_rect.y+enter_rect.h+15, window_width/2+200,
                                              enter_rect.y+enter_rect.h+60, 255, 255, 255, 255);
        SDL_RenderCopy(renderer, username_text_texture, NULL, &username_text_rect);
        if (show_warning) SDL_RenderCopy(renderer, warning_texture, NULL, &warning_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }

    TTF_CloseFont(username_font);
    TTF_CloseFont(enter_font);
    TTF_CloseFont(warning_font);
    SDL_FreeSurface(enter_surface);
    SDL_FreeSurface(username_text_surface);
    SDL_FreeSurface(warning_surface);
    SDL_DestroyTexture(warning_texture);
    SDL_DestroyTexture(username_text_texture);
    SDL_DestroyTexture(enter_texture);
    SDL_StopTextInput();
    return return_type;
}

void find_user(char* input_username)
{
    /*finding number of users*/
    FILE* number_of_users_file=fopen("../users_database/number_of_users.dat", "r");
    int number_of_users;
    fread(&number_of_users, sizeof(int), 1, number_of_users_file);
    fclose(number_of_users_file);
    FILE* users_file;
    if (number_of_users==0)
    {
        number_of_users_file=fopen("../users_database/number_of_users.dat", "w");
        number_of_users++;
        fwrite(&number_of_users, sizeof(int), 1, number_of_users_file);
        fclose(number_of_users_file);
        users_file=fopen("../users_database/users.dat", "w");
        main_user.score=0;
        strcpy(main_user.username, input_username);
        fwrite(&main_user, sizeof(user), number_of_users, users_file);
        fclose(number_of_users_file);
    }
    else
    {
        user all_users[number_of_users+1];
        users_file=fopen("../users_database/users.dat", "r");
        fread(all_users, sizeof(user), number_of_users, users_file);
        fclose(users_file);
        for (int i=0 ; i<number_of_users ; i++)
        {
            if (strcmp(all_users[i].username, input_username)==0)
            {
                main_user.score=all_users[i].score;
                strcpy(main_user.username, all_users[i].username);
                return;
            }
        }
        all_users[number_of_users].score=0;
        strcpy(all_users[number_of_users].username, input_username);
        number_of_users_file=fopen("../users_database/number_of_users.dat", "w");
        number_of_users++;
        fwrite(&number_of_users, sizeof(int), 1, number_of_users_file);
        fclose(number_of_users_file);
        users_file=fopen("../users_database/users.dat", "w");
        main_user.score=0;
        strcpy(main_user.username, input_username);
        fwrite(all_users, sizeof(user), number_of_users, users_file);
        fclose(number_of_users_file);
    }
}

void main_menu(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* bold_font)
{
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

    if (get_username(renderer, background_texture, background_rect)==game_quit)
    {
        SDL_DestroyEverything_main_menu(&background_texture, &button_texture, &texture_state_io, &new_game_text_texture, &resume_game_text_texture, &scoreboard_text_texture, &description_text_texture);
        return;
    }

    int close_requested=0;
    while(!close_requested)
    {
        SDL_RenderClear(renderer);
        SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case (SDL_QUIT):
                    SDL_DestroyEverything_main_menu(&background_texture, &button_texture, &texture_state_io, &new_game_text_texture, &resume_game_text_texture, &scoreboard_text_texture, &description_text_texture);
                    return;
                case (SDL_MOUSEBUTTONDOWN):
                    if (event.button.button!=SDL_BUTTON_LEFT) break;
                    if (mouse_position.x > new_game_button_rect.x && mouse_position.x < new_game_button_rect.x + new_game_button_rect.w &&
                        mouse_position.y > new_game_button_rect.y && mouse_position.y < new_game_button_rect.y + new_game_button_rect.h)
                        {
                            switch(new_game_menu(renderer, background_texture, button_texture, font, bold_font, background_rect, button_textColor))
                            {
                                case (game_quit):
                                    close_requested=1;
                                    break;
                                case (game_finished):
                                    break;
                            }
                        }
                    else if (mouse_position.x > resume_game_button_rect.x && mouse_position.x < resume_game_button_rect.x + resume_game_button_rect.w &&
                             mouse_position.y > resume_game_button_rect.y && mouse_position.y < resume_game_button_rect.y + resume_game_button_rect.h)
                        {
                            switch(resume_game(renderer, background_texture, button_texture, font, bold_font, background_rect))
                            {
                                case (game_quit):
                                    close_requested=1;
                                    break;
                            }
                        }
                    else if (mouse_position.x > scoreboard_button_rect.x && mouse_position.x < scoreboard_button_rect.x + scoreboard_button_rect.w &&
                             mouse_position.y > scoreboard_button_rect.y && mouse_position.x < scoreboard_button_rect.y + scoreboard_button_rect.h)
                        {
                            switch(view_scoreboard(renderer, background_texture, button_texture, font, bold_font, background_rect))
                            {
                                case (game_quit):
                                    close_requested=1;
                                    break;
                            }
                        }
                    break;
            }
        }
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
    SDL_DestroyEverything_main_menu(&background_texture, &button_texture, &texture_state_io, &new_game_text_texture, &resume_game_text_texture, &scoreboard_text_texture, &description_text_texture);
}

void SDL_DestroyEverything_main_menu(SDL_Texture** texture1, SDL_Texture** texture2, SDL_Texture** texture3, SDL_Texture** texture4, SDL_Texture** texture5, SDL_Texture** texture6, SDL_Texture** texture7)
{
    SDL_DestroyTexture(*texture1);
    SDL_DestroyTexture(*texture2);
    SDL_DestroyTexture(*texture3);
    SDL_DestroyTexture(*texture4);
    SDL_DestroyTexture(*texture5);
    SDL_DestroyTexture(*texture6);
    SDL_DestroyTexture(*texture7);
}