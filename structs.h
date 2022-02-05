#include "include.h"
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

typedef struct barracks
{
    int number_of_soldiers;
    int color;
}
barracks;

typedef struct potion
{
    int increase_soldiers_movement_speed;
    int decrease_enemy_soldiers_movement_speed;
    int enemy_soldiers_change_side;
    int increase_soldiers_generation_speed;
}
potion;

typedef struct country
{
    double x_center;
    double y_center;
    int color;
    int glow_flag;
    barracks barrack_in_country;
    int number_of_soldiers;
    int is_attacking;
    /*usage is for potions*/
    int soldiers_in_use;
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
    int attack_finished;
}
attack;