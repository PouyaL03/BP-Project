#include "map.h"

/*function prototypes*/
int start_game(SDL_Renderer*, TTF_Font*, TTF_Font*, country*);
void AI(country*, attack**);
void AI_attack(country*, attack**, int, int);
int check_if_game_is_finished(country*);
void win_lose_screen(SDL_Renderer*, int);
void update_user_score(country*, int);

int start_game(SDL_Renderer* renderer, TTF_Font* font,TTF_Font* bold_font, country* country_array)
{
    /*setting background color*/
    SDL_SetRenderDrawColor(renderer, colors[7].r, colors[7].g, colors[7].b, 255);
    SDL_Point mouse_position;
    int total_frames=0;
    attack* attack_head=NULL;
    int time_to_deploy=0;
    potion all_colors_potion[number_of_colors+1];
    for (int i=0 ; i<number_of_colors+1 ; i++)
    {
        all_colors_potion[i].enable=0;
        all_colors_potion[i].time=0;
        all_colors_potion[i].x_center=-40;
        all_colors_potion[i].y_center=-40;
        for (int j=0 ; j<number_of_potions ; j++)
        {
            all_colors_potion[i].type[j]=0;
        }
    }
    /*last potion is the potion on the screen*/
    /*------------------------------------------------------------------------------------*/
    while (1)
    {
        SDL_Event event;
        switch(event_handling(&event, country_array, mouse_position, &attack_head))
        {
            case(game_quit):
                return game_quit;
        }
        update_number_of_soldiers(country_array, total_frames, all_colors_potion);
        SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
        check_mouse_state(mouse_position, country_array, initial_side_length*sin(teta));
        SDL_SetRenderDrawColor(renderer,
                               colors[background_color].r,
                               colors[background_color].g,
                               colors[background_color].b, 255);
        SDL_RenderClear(renderer);
        update_attacking_soldiers_position(country_array, &attack_head, total_frames, all_colors_potion);
        potion_logic(country_array, &attack_head, total_frames, &time_to_deploy, all_colors_potion);
        AI(country_array, &attack_head);
        draw_map(renderer, country_array, bold_font, font, &attack_head, all_colors_potion);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
        switch(check_if_game_is_finished(country_array))
        {
            case (win):
                win_lose_screen(renderer, win);
                update_user_score(country_array, win);
                return game_finished;
            case (lose):
                win_lose_screen(renderer, lose);
                update_user_score(country_array, lose);
                return game_finished;
        }
        total_frames++;
    }
    return game_finished;
}

void update_user_score(country* country_array, int win_lose)
{
    if (win_lose==win)
    {
        main_user.score+=5;
    }
    else
    {
        main_user.score-=3;
    }
    /*finding number of users*/
    FILE* number_of_users_file=fopen("../users_database/number_of_users.dat", "r");
    int number_of_users;
    fread(&number_of_users, sizeof(int), 1, number_of_users_file);
    fclose(number_of_users_file);
    user all_users[number_of_users];
    FILE* users_file=fopen("../users_database/users.dat", "r");
    fread(all_users, sizeof(user), number_of_users, users_file);
    fclose(users_file);
    for (int i=0 ; i<number_of_users ; i++)
    {
        if (strcmp(all_users[i].username, main_user.username)==0)
        {
            all_users[i].score=main_user.score;
            break;
        }
    }
    users_file=fopen("../users_database/users.dat", "w");
    fwrite(all_users, sizeof(user), number_of_users, users_file);
    fclose(users_file);
}

int check_if_game_is_finished(country* country_array)
{
    int all_blue=1;
    int no_blue=1;
    for (int i=0 ; i<number_of_countries ; i++)
    {
        if (country_array[i].color!=unallocated_color && country_array[i].color!=no_player_color)
        {
            if (country_array[i].color==blue) continue;
            else all_blue=0;
        }
    }
    if (all_blue) return win;
    for (int i=0 ; i<number_of_countries ; i++)
    {
        if (country_array[i].color!=blue) continue;
        else no_blue=0;
    }
    if (no_blue) return lose;
    return continue_game;
}

void win_lose_screen(SDL_Renderer* renderer, int win_lose)
{
    char line[10];
    line[0]='Y'; line[1]='O'; line[2]='U'; line[3]=' '; line[4]='\0';
    if (win_lose==win) strcat(line, "WON");
    else strcat(line, "LOST");
    SDL_Event event;
    TTF_Font* win_lose_text_font=TTF_OpenFont("../fonts/TNR_B.ttf", 150);
    SDL_Color win_lose_text_color;
    if (win_lose==win)
    {
        win_lose_text_color.r=0;
        win_lose_text_color.g=255;
        win_lose_text_color.b=0;
    }
    else
    {
        win_lose_text_color.r=255;
        win_lose_text_color.g=0;
        win_lose_text_color.b=0;
    }
    win_lose_text_color.a=255;
    SDL_Surface* win_lose_text_surface=TTF_RenderText_Solid(win_lose_text_font, line, win_lose_text_color);
    SDL_Texture* win_lose_text_texture=SDL_CreateTextureFromSurface(renderer, win_lose_text_surface);
    SDL_Rect win_lose_text_rect;
    win_lose_text_rect.x=(window_width-win_lose_text_surface->w)/2;
    win_lose_text_rect.y=200;
    win_lose_text_rect.h=win_lose_text_surface->h;
    win_lose_text_rect.w=win_lose_text_surface->w;
    TTF_Font* any_key_font=TTF_OpenFont("../fonts/TNR.ttf", 50);
    SDL_Color any_key_color={255, 255, 255, 255};
    SDL_Surface* any_key_surface=TTF_RenderText_Solid(any_key_font, "please press any key to continue", any_key_color);
    SDL_Texture* any_key_texture=SDL_CreateTextureFromSurface(renderer, any_key_surface);
    SDL_Rect any_key_rect;
    any_key_rect.x=(window_width-any_key_surface->w)/2;
    any_key_rect.y=700;
    any_key_rect.h=any_key_surface->h;
    any_key_rect.w=any_key_surface->w;
    int close_requested=0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    while (!close_requested)
    {
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case (SDL_QUIT):
                    close_requested=1;
                    break;
                case (SDL_KEYDOWN):
                    close_requested=1;
                    break;
            }
        }
        SDL_RenderCopy(renderer, win_lose_text_texture, NULL, &win_lose_text_rect);
        SDL_RenderCopy(renderer, any_key_texture, NULL, &any_key_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }
    TTF_CloseFont(any_key_font);
    TTF_CloseFont(win_lose_text_font);
    SDL_FreeSurface(win_lose_text_surface);
    SDL_FreeSurface(any_key_surface);
    SDL_DestroyTexture(win_lose_text_texture);
    SDL_DestroyTexture(any_key_texture);
}

void AI_attack(country* country_array, attack** attack_head, int attacker, int defender)
{
    if (*attack_head==NULL)
    {
        (*attack_head)=malloc(sizeof(attack));
        (*attack_head)->attack_complete=1;
        (*attack_head)->attacking_country_index=attacker;
        (*attack_head)->defenfing_country_index=defender;
        (*attack_head)->next_attack=NULL;
        (*attack_head)->soldier_head=malloc(sizeof(soldier));
        initialize_soldiers(attack_head, country_array);
        country_array[attacker].soldiers_in_use+=country_array[attacker].number_of_soldiers;
        country_array[attacker].number_of_soldiers=0;
    }
    else
    {
        attack* tmp=(*attack_head);
        while(tmp->next_attack!=NULL)
        {
            tmp=tmp->next_attack;
        }
        tmp->next_attack=malloc(sizeof(attack));
        tmp->next_attack->attack_complete=1;
        tmp->next_attack->attacking_country_index=attacker;
        tmp->next_attack->defenfing_country_index=defender;
        tmp->next_attack->next_attack=NULL;
        tmp->next_attack->soldier_head=malloc(sizeof(soldier));
        initialize_soldiers(&(tmp->next_attack), country_array);
        country_array[attacker].soldiers_in_use+=country_array[attacker].number_of_soldiers;
        country_array[attacker].number_of_soldiers=0;
    }
}

void AI(country* country_array, attack** attack_head)
{
    for (int attacking_country_index=0 ; attacking_country_index<number_of_countries ; attacking_country_index++)
    {
        if (country_array[attacking_country_index].color==blue || country_array[attacking_country_index].color==unallocated_color || country_array[attacking_country_index].color==no_player_color) continue;
        if (country_array[attacking_country_index].number_of_soldiers==0) continue;
        for (int defending_country_index=0 ; defending_country_index<number_of_countries ; defending_country_index++)
        {
            if (attacking_country_index==defending_country_index) continue;
            if (country_array[defending_country_index].color==unallocated_color) continue;
            if (country_array[defending_country_index].color==country_array[attacking_country_index].color) continue;
            if (country_array[defending_country_index].color==no_player_color)
            {
                if (rand()%15000==0)
                {
                    if (country_array[attacking_country_index].number_of_soldiers>country_array[defending_country_index].number_of_soldiers)
                    {
                        AI_attack(country_array, attack_head, attacking_country_index, defending_country_index);
                    }
                }
            }
            else
            {
                if (rand()%15000==0)
                {
                    if (country_array[attacking_country_index].number_of_soldiers>country_array[defending_country_index].number_of_soldiers)
                    {
                        AI_attack(country_array, attack_head, attacking_country_index, defending_country_index);
                    }
                }
            }
        }
    }
}