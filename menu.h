#include "game.h"
#include <SDL_ttf.h>

/*function prototypes*/
int new_game_menu(SDL_Renderer*, SDL_Texture*, SDL_Texture*, TTF_Font*, TTF_Font*, SDL_Rect, SDL_Color);
int resume_game(SDL_Renderer*, SDL_Texture*, SDL_Texture*, TTF_Font*, TTF_Font*, SDL_Rect);
int view_scoreboard(SDL_Renderer*, SDL_Texture*, SDL_Texture*, TTF_Font*, TTF_Font*, SDL_Rect);
int choose_map(SDL_Renderer*, SDL_Texture*, SDL_Texture*, TTF_Font*, TTF_Font*, SDL_Rect, SDL_Color);
void SDL_DestroyEverything_new_game_menu(SDL_Texture**, SDL_Texture**, TTF_Font**);
void SDL_DestroyEverything_choose_map(map*, int, TTF_Font**);
void SDL_DestroyEverything_resume_game_menu();
void SDL_DestroyEverything_view_scoreboard();

int choose_map(SDL_Renderer* renderer, SDL_Texture* background_texture, SDL_Texture* button_texture, TTF_Font* font, TTF_Font* bold_font, SDL_Rect background_rect, SDL_Color button_textColor)
{
    SDL_Event event;
    SDL_Point mouse_position;
    TTF_Font* button_text_font = TTF_OpenFont("../fonts/TNR_B.ttf", 40);
    FILE* number_of_maps_file = fopen("../maps/number_of_maps.dat", "r");
    int number_of_maps;
    fread(&number_of_maps, sizeof(int), 1, number_of_maps_file);
    fclose(number_of_maps_file);
    map all_maps[number_of_maps];
    sprintf(all_maps[0].name, "TEST MAP");
    for (int i=1 ; i<number_of_maps ; i++)
    {
        all_maps[i].name[0]='M';
        all_maps[i].name[1]='A';
        all_maps[i].name[2]='P';
        sprintf(all_maps[i].name+3, "%d", i);
    }
    for (int i=0 ; i<number_of_maps ; i++)
    {
        all_maps[i].text_surface=TTF_RenderText_Solid(button_text_font, all_maps[i].name, button_textColor);
        all_maps[i].text_texture=SDL_CreateTextureFromSurface(renderer, all_maps[i].text_surface);
        all_maps[i].button_rect.h=button_height;
        all_maps[i].button_rect.w=button_width;
        all_maps[i].button_rect.x=800;
        all_maps[i].button_rect.y=100+100*i;
        all_maps[i].text_rect.x=all_maps[i].button_rect.x+(all_maps[i].button_rect.w-all_maps[i].text_surface->w)/2;
        all_maps[i].text_rect.y=all_maps[i].button_rect.y+(all_maps[i].button_rect.h-all_maps[i].text_surface->h)/2;
        all_maps[i].text_rect.w=all_maps[i].text_surface->w;
        all_maps[i].text_rect.h=all_maps[i].text_surface->h;
    }
    int close_requested=0;
    while (!close_requested)
    {
        SDL_RenderClear(renderer);
        SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case (SDL_QUIT):
                    close_requested=1;
                    SDL_DestroyEverything_choose_map(all_maps, number_of_maps, &button_text_font);
                    return game_quit;
                case (SDL_MOUSEBUTTONDOWN):
                    if (event.button.button!=SDL_BUTTON_LEFT) break;
                    for (int i=0 ; i<number_of_maps ; i++)
                    {
                        if (mouse_position.x > all_maps[i].button_rect.x && mouse_position.x < all_maps[i].button_rect.x + all_maps[i].button_rect.w &&
                            mouse_position.y > all_maps[i].button_rect.y && mouse_position.y < all_maps[i].button_rect.y + all_maps[i].button_rect.h)
                            {
                                country country_array[maximum_number_of_countries];
                                unsigned char file_name[30];
                                sprintf(file_name, "../maps/map_data/");
                                if(i!=0) sprintf(file_name+strlen(file_name), all_maps[i].name);
                                else sprintf(file_name+strlen(file_name), "MAP0");
                                sprintf(file_name+strlen(file_name), ".dat");
                                FILE* map_file=fopen(file_name, "r");
                                number_of_countries=fread(country_array, sizeof(country), maximum_number_of_countries, map_file);
                                fclose(map_file);
                                switch(start_game(renderer, font, bold_font, country_array))
                                {
                                    case(game_quit):
                                        SDL_DestroyEverything_choose_map(all_maps, number_of_maps, &button_text_font);
                                        return game_quit;
                                    case(game_finished):
                                        SDL_DestroyEverything_choose_map(all_maps, number_of_maps, &button_text_font);
                                        return game_finished;
                                }
                                break;
                            }
                    }
            }
        }
        SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);
        for (int i=0 ; i<number_of_maps ; i++)
        {
            SDL_RenderCopy(renderer, button_texture, NULL, &all_maps[i].button_rect);
            SDL_RenderCopy(renderer, all_maps[i].text_texture, NULL, &all_maps[i].text_rect);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }
    SDL_DestroyEverything_choose_map(all_maps, number_of_maps, &button_text_font);
    return nothing;
}

int new_game_menu(SDL_Renderer* renderer, SDL_Texture* background_texture, SDL_Texture* button_texture, TTF_Font* font, TTF_Font* bold_font, SDL_Rect background_rect, SDL_Color button_textColor)
{
    SDL_Event event;
    SDL_Point mouse_position;
    TTF_Font* button_text_font=TTF_OpenFont("../fonts/TNR_B.ttf", 40);
    SDL_Surface* choose_map_text_surface=TTF_RenderText_Solid(button_text_font, "CHOOSE MAP", button_textColor);
    SDL_Texture* choose_map_text_texture=SDL_CreateTextureFromSurface(renderer, choose_map_text_surface);
    SDL_Surface* random_map_text_surface=TTF_RenderText_Solid(button_text_font, "RANDOM MAP", button_textColor);
    SDL_Texture* random_map_text_texture=SDL_CreateTextureFromSurface(renderer, random_map_text_surface);
    SDL_Rect choose_map_button_rect;
    choose_map_button_rect.h=button_height;
    choose_map_button_rect.w=button_width;
    choose_map_button_rect.x=(window_width-button_width)/2;
    choose_map_button_rect.y=400;
    SDL_Rect choose_map_text_rect;
    choose_map_text_rect.x=choose_map_button_rect.x+(choose_map_button_rect.w-choose_map_text_surface->w)/2;
    choose_map_text_rect.y=choose_map_button_rect.y+(choose_map_button_rect.h-choose_map_text_surface->h)/2;
    choose_map_text_rect.w=choose_map_text_surface->w;
    choose_map_text_rect.h=choose_map_text_surface->h;
    SDL_Rect random_map_button_rect;
    random_map_button_rect.h=button_height;
    random_map_button_rect.w=button_width;
    random_map_button_rect.x=(window_width-button_width)/2;
    random_map_button_rect.y=500;
    SDL_Rect random_map_text_rect;
    random_map_text_rect.x=random_map_button_rect.x+(random_map_button_rect.w-random_map_text_surface->w)/2;
    random_map_text_rect.y=random_map_button_rect.y+(random_map_button_rect.h-random_map_text_surface->h)/2;
    random_map_text_rect.w=random_map_text_surface->w;
    random_map_text_rect.h=random_map_text_surface->h;
    SDL_FreeSurface(choose_map_text_surface);
    SDL_FreeSurface(random_map_text_surface);
    int close_requested=0;
    while (!close_requested)
    {
        SDL_RenderClear(renderer);
        SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case (SDL_QUIT):
                    close_requested=1;
                    SDL_DestroyEverything_new_game_menu(&choose_map_text_texture, &random_map_text_texture, &button_text_font);
                    return game_quit;
                case (SDL_MOUSEBUTTONDOWN):
                    if (event.button.button!=SDL_BUTTON_LEFT) break;
                    if (mouse_position.x > random_map_button_rect.x && mouse_position.x < random_map_button_rect.x + random_map_button_rect.w &&
                        mouse_position.y > random_map_button_rect.y && mouse_position.y < random_map_button_rect.y + random_map_button_rect.h)
                        {
                            //initializing countires
                            country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row];
                            create_random_map(all_countries);
                            country country_array[number_of_countries];
                            initialize_country_array(country_array, all_countries);
                            switch(start_game(renderer, font, bold_font, country_array))
                            {
                                case(game_quit):
                                    SDL_DestroyEverything_new_game_menu(&random_map_text_texture, &choose_map_text_texture, &button_text_font);
                                    return game_quit;
                                case(game_finished):
                                    SDL_DestroyEverything_new_game_menu(&random_map_text_texture, &choose_map_text_texture, &button_text_font);
                                    return game_finished;
                            }
                        }
                    else if (mouse_position.x > choose_map_button_rect.x && mouse_position.x < choose_map_button_rect.x + choose_map_button_rect.w &&
                             mouse_position.y > choose_map_button_rect.y && mouse_position.y < choose_map_button_rect.y + choose_map_button_rect.h)
                            {
                                switch(choose_map(renderer, background_texture, button_texture, font, bold_font, background_rect, button_textColor))
                                {
                                    case(game_quit):
                                        SDL_DestroyEverything_new_game_menu(&random_map_text_texture, &choose_map_text_texture, &button_text_font);
                                        return game_quit;
                                    case(game_finished):
                                        SDL_DestroyEverything_new_game_menu(&random_map_text_texture, &choose_map_text_texture, &button_text_font);
                                        return game_finished;
                                }
                            }
            }
        }
        SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);
        SDL_RenderCopy(renderer, button_texture, NULL, &choose_map_button_rect);
        SDL_RenderCopy(renderer, button_texture, NULL, &random_map_button_rect);
        SDL_RenderCopy(renderer, choose_map_text_texture, NULL, &choose_map_text_rect);
        SDL_RenderCopy(renderer, random_map_text_texture, NULL, &random_map_text_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }
    SDL_DestroyEverything_new_game_menu(&choose_map_text_texture, &random_map_text_texture, &button_text_font);
    return nothing;
}

int resume_game(SDL_Renderer* renderer, SDL_Texture* background_texture, SDL_Texture* button_texture, TTF_Font* font, TTF_Font* bold_font, SDL_Rect background_rect)
{
    TTF_Font* tmp_font=TTF_OpenFont("../fonts/TNR_B.ttf", 150);
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
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
    TTF_CloseFont(tmp_font);
    SDL_DestroyTexture(tmp_texture);
    SDL_FreeSurface(tmp_surface);
    return nothing;
}

int view_scoreboard(SDL_Renderer* renderer, SDL_Texture* background_texture, SDL_Texture* button_texture, TTF_Font* font, TTF_Font* bold_font, SDL_Rect background_rect)
{
    int list_width=window_width-300;
    int list_height=175;
    int return_type=nothing;
    SDL_Event event;
    SDL_Point mouse_position;
    FILE* number_of_users_file=fopen("../users_database/number_of_users.dat", "r");
    int number_of_users;
    fread(&number_of_users, sizeof(int), 1, number_of_users_file);
    fclose(number_of_users_file);
    user all_users[number_of_users];
    FILE* users_file=fopen("../users_database/users.dat", "r");
    fread(all_users, sizeof(user), number_of_users, users_file);
    fclose(users_file);
    int number_of_users_on_scoreboard=number_of_users;
    if (number_of_users_on_scoreboard>5) number_of_users_on_scoreboard=5;
    user all_users_on_scoreboard[number_of_users_on_scoreboard];
    SDL_Texture* username_texture[number_of_users_on_scoreboard+1];
    SDL_Surface* username_surface[number_of_users_on_scoreboard+1];
    SDL_Texture* score_texture[number_of_users_on_scoreboard+1];
    SDL_Surface* score_surface[number_of_users_on_scoreboard+1];
    SDL_Rect score_rect[number_of_users_on_scoreboard+1];
    SDL_Rect username_rect[number_of_users_on_scoreboard+1];
    SDL_Rect button_rect[number_of_users_on_scoreboard];
    
    TTF_Font* main_font=TTF_OpenFont("../fonts/TNR_B.ttf", 60);
    SDL_Color main_color={255, 255, 255, 255};
    printf("%d\n", number_of_users_on_scoreboard);

    for (int i=0 ; i<number_of_users_on_scoreboard ; i++)
    {
        button_rect[i].w=list_width;
        button_rect[i].h=list_height;
        button_rect[i].y=175+150*i;
        button_rect[i].x=(window_width-list_width)/2;
    }

    all_users_on_scoreboard[0].score=all_users[0].score;
    strcpy(all_users_on_scoreboard[0].username, all_users[0].username);

    for (int i=1 ; i<number_of_users ; i++)
    {
        if (all_users[i].score>all_users_on_scoreboard[0].score)
        {
            all_users_on_scoreboard[0].score=all_users[i].score;
            strcpy(all_users_on_scoreboard[0].username, all_users[i].username);
        }
    }

    for (int i=1 ; i<number_of_users_on_scoreboard ; i++)
    {
        all_users_on_scoreboard[i].score=-1000000;
        all_users_on_scoreboard[i].username[0]='\0';
        all_users_on_scoreboard[i].username[1]='\0';
        for (int j=0 ; j<number_of_users ; j++)
        {
            if (all_users[j].score>=all_users_on_scoreboard[i].score)
            {
                if (all_users[j].score<=all_users_on_scoreboard[i-1].score)
                {
                    if (strcmp(all_users[j].username, all_users_on_scoreboard[i-1].username)!=0)
                    {
                        all_users_on_scoreboard[i].score=all_users[j].score;
                        strcpy(all_users_on_scoreboard[i].username, all_users[j].username);
                    }
                }
            }
        }
    }

    for (int i=0 ; i<number_of_users_on_scoreboard ; i++)
    {
        username_surface[i]=TTF_RenderText_Solid(main_font, all_users_on_scoreboard[i].username, main_color);
        username_texture[i]=SDL_CreateTextureFromSurface(renderer, username_surface[i]);
        username_rect[i].h=username_surface[i]->h;
        username_rect[i].w=username_surface[i]->w;
        username_rect[i].x=button_rect[i].x+80;
        username_rect[i].y=button_rect[i].y+(button_rect[i].h-username_rect[i].h)/2-10;
        char score[10];
        score[0]='\0';
        sprintf(score, "%d", all_users_on_scoreboard[i].score);
        score_surface[i]=TTF_RenderText_Solid(main_font, score, main_color);
        score_texture[i]=SDL_CreateTextureFromSurface(renderer, score_surface[i]);
        score_rect[i].h=score_surface[i]->h;
        score_rect[i].w=score_surface[i]->w;
        score_rect[i].x=button_rect[i].x+button_rect[i].w-score_rect[i].w-140;
        score_rect[i].y=button_rect[i].y+(button_rect[i].h-score_rect[i].h)/2-10;
    }

    TTF_Font* head_font=TTF_OpenFont("../fonts/TNR_B.ttf", 60);
    SDL_Color head_color={255, 0, 0, 255};
    username_surface[number_of_users_on_scoreboard]=TTF_RenderText_Solid(head_font, "USERNAME", head_color);
    username_texture[number_of_users_on_scoreboard]=SDL_CreateTextureFromSurface(renderer, username_surface[number_of_users_on_scoreboard]);
    username_rect[number_of_users_on_scoreboard].h=username_surface[number_of_users_on_scoreboard]->h;
    username_rect[number_of_users_on_scoreboard].w=username_surface[number_of_users_on_scoreboard]->w;
    username_rect[number_of_users_on_scoreboard].x=button_rect[0].x+60;
    username_rect[number_of_users_on_scoreboard].y=125;
    score_surface[number_of_users_on_scoreboard]=TTF_RenderText_Solid(head_font, "SCORE", head_color);
    score_texture[number_of_users_on_scoreboard]=SDL_CreateTextureFromSurface(renderer, score_surface[number_of_users_on_scoreboard]);
    score_rect[number_of_users_on_scoreboard].h=score_surface[number_of_users_on_scoreboard]->h;
    score_rect[number_of_users_on_scoreboard].w=score_surface[number_of_users_on_scoreboard]->w;
    score_rect[number_of_users_on_scoreboard].x=button_rect[0].x+button_rect[0].w-score_rect[number_of_users_on_scoreboard].w-100;
    score_rect[number_of_users_on_scoreboard].y=125;
    int close_requested=0;
    while(!close_requested)
    {
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case (SDL_QUIT):
                    close_requested=1;
                    return_type=game_quit;
                    break;
            }
        }
        SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);
        for (int i=0 ; i<number_of_users_on_scoreboard ; i++)
        {
            SDL_RenderCopy(renderer, button_texture, NULL, &button_rect[i]);
        }
        for (int i=0 ; i<number_of_users_on_scoreboard+1 ; i++)
        {
            SDL_RenderCopy(renderer, username_texture[i], NULL, &username_rect[i]);
            SDL_RenderCopy(renderer, score_texture[i], NULL, &score_rect[i]);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }
    SDL_DestroyEverything_view_scoreboard();
    return return_type;
}

void SDL_DestroyEverything_new_game_menu(SDL_Texture** texture1, SDL_Texture** texture2, TTF_Font** font1)
{
    SDL_DestroyTexture(*texture1);
    SDL_DestroyTexture(*texture2);
    TTF_CloseFont(*font1);
}

void SDL_DestroyEverything_choose_map(map* all_maps, int number_of_maps, TTF_Font** font1)
{
    for (int i=0 ; i<number_of_maps ; i++)
    {
        SDL_DestroyTexture(all_maps[i].text_texture);
        SDL_FreeSurface(all_maps[i].text_surface);
    }
    TTF_CloseFont(*font1);
}

void SDL_DestroyEverything_view_scoreboard()
{

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