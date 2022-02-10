#include "map.h"

/*function prototypes*/
int start_game(SDL_Renderer*, TTF_Font*, TTF_Font*, country*);
void AI(country*, attack**);
void AI_attack(country*, attack**, int, int);

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
        total_frames++;
    }
    return game_finished;
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
    // /*select color of background*/
    // SDL_Surface* surface = IMG_Load("../images/sample-picture.png");
    // /*loading the texture to the windows*/
    // SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_FreeSurface(surface);
    // /*creating a single rectangle to save the pic into it*/
    // SDL_Rect destination;
    // SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
    // destination.w/=4;
    // destination.h/=4;
    // float x_position=(window_width-destination.w)/2;
    // float y_position=(window_height-destination.h)/2;