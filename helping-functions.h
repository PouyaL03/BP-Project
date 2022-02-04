#include "constants.h"

/*function prototypes*/
void draw_hexagon_borderline(SDL_Renderer*, double, double, double, int, Uint8);
void draw_hexagon_no_borderline(SDL_Renderer*, double, double, double, int, Uint8);
void initialize_country_array(country country_array[number_of_countries],
                              country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row]);
void check_mouse_state(SDL_Point, country*, double);
void update_number_of_soldiers(country*, int);
int find_country_index(country*, SDL_Point, double);
void event_handling(SDL_Event*, country*, SDL_Point, int*, attack**);
void update_attacking_soldiers_position(country*, attack**, int);
void check_for_collisions(attack**);

void check_for_collisions(attack** attack_head)
{

}

void update_attacking_soldiers_position(country* country_array, attack** attack_head, int total_frames)
{
    attack* tmp_attack_head=(*attack_head);
    for ( ; tmp_attack_head!=NULL ; tmp_attack_head=tmp_attack_head->next_attack)
    {
        if (tmp_attack_head->attack_complete==0) continue;
        soldier* tmp_soldier_head=tmp_attack_head->soldier_head;
        int defending_x=country_array[tmp_attack_head->defenfing_country_index].x_center;
        int defending_y=country_array[tmp_attack_head->defenfing_country_index].y_center;
        int attacking_x=country_array[tmp_attack_head->attacking_country_index].x_center;
        int attacking_y=country_array[tmp_attack_head->attacking_country_index].y_center;
        double sinus=(defending_y-attacking_y)*10.0/(sqrt((defending_x-attacking_x)*(defending_x-attacking_x)+(defending_y-attacking_y)*(defending_y-attacking_y))*10);
        double cosinus=(defending_x-attacking_x)*10.0/(sqrt((defending_x-attacking_x)*(defending_x-attacking_x)+(defending_y-attacking_y)*(defending_y-attacking_y))*10);
        for ( ; tmp_soldier_head!=NULL ; tmp_soldier_head=tmp_soldier_head->next_soldier)
        {
            tmp_soldier_head->soldier_position.x+=initial_speed_of_players*cosinus;
            tmp_soldier_head->soldier_position.y+=initial_speed_of_players*sinus;
            if (tmp_soldier_head->next_soldier!=NULL)
            {

            }
        }
    }
    check_for_collisions(attack_head);
}

void event_handling(SDL_Event* event, country* country_array, SDL_Point mouse_position, int* close_requested, attack** attack_head)
{
    while(SDL_PollEvent(event))
        {
            switch (event->type)
            {
                case (SDL_QUIT):
                    printf("khaste nabashid.\n");
                    fflush(stdin);
                    *close_requested=1;
                    break;
                case (SDL_MOUSEBUTTONDOWN):
                    if (event->button.button==SDL_BUTTON_LEFT)
                    {
                        int which_country=find_country_index(country_array, mouse_position, initial_side_length*sin(teta));
                        if (which_country!=-1)
                        {    
                            if (waiting_for_attack==0)
                            {
                                if (country_array[which_country].color!=unallocated_color && country_array[which_country].color!=no_player_color)
                                {
                                    waiting_for_attack=1;
                                    attack* tmp=malloc(sizeof(attack));
                                    if ((*attack_head)==NULL)
                                    {
                                        (*attack_head)=tmp;
                                        (*attack_head)->next_attack=NULL;
                                    }
                                    else
                                    {
                                        tmp->next_attack=(*attack_head);
                                        (*attack_head)=tmp;
                                    }
                                    (*attack_head)->attacking_country_index=which_country;
                                    (*attack_head)->attack_complete=0;
                                    (*attack_head)->defenfing_country_index=-1;
                                    (*attack_head)->attack_finished=0;
                                }
                            }
                            else
                            {
                                if (which_country!=-1 && country_array[which_country].color!=unallocated_color && which_country!=(*attack_head)->attacking_country_index)
                                {
                                    waiting_for_attack=0;
                                    (*attack_head)->attack_complete=1;
                                    (*attack_head)->defenfing_country_index=which_country;
                                    int tmp_maximum_soldiers_in_attack=country_array[(*attack_head)->attacking_country_index].number_of_soldiers-
                                                                   country_array[(*attack_head)->attacking_country_index].soldiers_in_use;
                                    country_array[(*attack_head)->attacking_country_index].soldiers_in_use+=tmp_maximum_soldiers_in_attack;
                                    (*attack_head)->maximum_soldiers_in_attack=tmp_maximum_soldiers_in_attack;
                                    (*attack_head)->soldier_head=(soldier*)malloc(sizeof(soldier));
                                    (*attack_head)->soldier_head->next_soldier=NULL;
                                    (*attack_head)->soldier_head->color=country_array[(*attack_head)->attacking_country_index].color;
                                    (*attack_head)->soldier_head->soldier_position.x=country_array[(*attack_head)->attacking_country_index].x_center;
                                    (*attack_head)->soldier_head->soldier_position.y=country_array[(*attack_head)->attacking_country_index].y_center;
                                }
                            }
                        }
                    }
                    else if (event->button.button==SDL_BUTTON_RIGHT)
                    {
                        if ((*attack_head)!=NULL)
                        {
                            if ((*attack_head)->attack_complete==0)
                            {
                                waiting_for_attack=0;
                                (*attack_head)=(*attack_head)->next_attack;
                            }
                        }
                    }
                    /*for debugging purposes*/
                    else if (event->button.button==SDL_BUTTON_MIDDLE)
                    {
                        attack* tmp;
                        tmp=(*attack_head);
                        while(tmp!=NULL)
                        {
                            printf("attacking country is:%d\n", tmp->attacking_country_index);
                            printf("defending country is:%d\n", tmp->defenfing_country_index);
                            fflush(stdin);
                            tmp=tmp->next_attack;
                        }
                        printf("-----------------------------------------\n");
                        fflush(stdin);
                    }
                    break;
            }
        }
}

int find_country_index(country* country_array, SDL_Point mouse_position, double check_radius)
{
    for (int i=0 ; i<number_of_countries ; i++)
    {
        if ((country_array[i].x_center-mouse_position.x)*(country_array[i].x_center-mouse_position.x)+
            (country_array[i].y_center-mouse_position.y)*(country_array[i].y_center-mouse_position.y)<=
             check_radius*check_radius) return i;
    }
    return -1;
}

void update_number_of_soldiers(country* country_array, int total_frames)
{
    for (int i=0 ; i<number_of_countries ; i++)
    {
        if(country_array[i].number_of_soldiers==maximum_number_of_soldiers) continue;
        if(country_array[i].color==unallocated_color || country_array[i].color==no_player_color) continue;
        if(total_frames%FPS==0) country_array[i].number_of_soldiers++;
    }
}

void check_mouse_state(SDL_Point mouse_position, country country_array[number_of_countries], double check_radius)
{
    for (int i=0 ; i<number_of_countries ; i++)
    {
        if ((country_array[i].x_center-mouse_position.x)*(country_array[i].x_center-mouse_position.x) +
             (country_array[i].y_center-mouse_position.y)*(country_array[i].y_center-mouse_position.y) 
             < check_radius*check_radius) country_array[i].glow_flag=1;
             else country_array[i].glow_flag=0;
    }
}

void draw_hexagon_borderline(SDL_Renderer* renderer, double x_center, double y_center, double side_length, int which_color, Uint8 alpha)
{
    Sint16 vx[6];
    Sint16 vy[6];
    vx[0]=x_center-side_length*cos(teta); vy[0]=y_center+side_length*sin(teta);
    vx[1]=x_center+side_length*cos(teta); vy[1]=y_center+side_length*sin(teta);
    vx[2]=x_center+side_length; vy[2]=y_center;
    vx[3]=x_center+side_length*cos(teta); vy[3]=y_center-side_length*sin(teta);
    vx[4]=x_center-side_length*cos(teta); vy[4]=y_center-side_length*sin(teta);
    vx[5]=x_center-side_length; vy[5]=y_center;
    filledPolygonRGBA(renderer, vx, vy, 6, colors[which_color].r,
                                           colors[which_color].g,
                                           colors[which_color].b,
                                           alpha);
    aapolygonRGBA(renderer, vx, vy, 6, 0, 0, 0, alpha);
}

void draw_hexagon_no_borderline(SDL_Renderer* renderer, double x_center, double y_center, double side_length, int which_color, Uint8 alpha)
{
    Sint16 vx[6];
    Sint16 vy[6];
    vx[0]=x_center-side_length*cos(teta); vy[0]=y_center+side_length*sin(teta);
    vx[1]=x_center+side_length*cos(teta); vy[1]=y_center+side_length*sin(teta);
    vx[2]=x_center+side_length; vy[2]=y_center;
    vx[3]=x_center+side_length*cos(teta); vy[3]=y_center-side_length*sin(teta);
    vx[4]=x_center-side_length*cos(teta); vy[4]=y_center-side_length*sin(teta);
    vx[5]=x_center-side_length; vy[5]=y_center;
    filledPolygonRGBA(renderer, vx, vy, 6, colors[which_color].r,
                                           colors[which_color].g,
                                           colors[which_color].b,
                                           alpha);
}

void initialize_country_array(country country_array[number_of_countries],
                              country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row])
{
    for (int i=0 ; i<number_of_hexagons_in_column ; i++)
    {
        for (int j=0 ; j<number_of_hexagons_in_row ; j++)
        {
            country_array[i*number_of_hexagons_in_row+j]=all_countries[i][j];
        }
    }
}