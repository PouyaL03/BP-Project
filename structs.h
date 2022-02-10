#include "include.h"

typedef struct user
{
    char username[20];
    int score;
}
user;

typedef struct soldier
{
    int color;
    double soldier_position_x;
    double soldier_position_y;
    SDL_Point soldier_position;
    int already_counted;
    struct soldier* next_soldier;
}
soldier;

typedef struct potion
{
    int type[4];
    int enable;
    int time;
    int x_center;
    int y_center;
}
potion;

typedef struct country
{
    double x_center;
    double y_center;
    int color;
    int glow_flag;
    int number_of_soldiers;
    int soldiers_in_use;
    /*usage is for potions*/
}
country;

typedef struct attack
{
    int attack_complete;
    int attacking_country_index;
    int defenfing_country_index;
    soldier* soldier_head;
    struct attack* next_attack;
    int maximum_soldiers_in_attack;
    int total_soldiers_in_attack;
    // int attack_finished;
}
attack;

typedef struct map
{
    SDL_Surface* text_surface;
    SDL_Texture* text_texture;
    SDL_Rect button_rect;
    SDL_Rect text_rect;
    char name[10];
}
map;