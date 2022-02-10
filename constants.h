#include "structs.h"
char player_name[100];
#define window_width 1280
#define window_height 960
#define button_width window_width/3
#define button_height 100
#define number_of_hexagons_in_row 7
#define number_of_hexagons_in_column 5
int number_of_countries=0;
#define maximum_number_of_countries 35
double initial_side_length=70;
double radius_of_circle=20;
const int FPS=24;
#define number_of_colors 8
#define pi M_PI
#define teta pi/3
#define maximum_number_of_soldiers 90
int waiting_for_attack=0;
double initial_speed_of_players=7;
/*space_between_two_soldiers=4*initial_speed_of_players*/
double space_between_two_soldiers=60;
double initial_distance_between_players=40;
int potion_time_on_screen=5;

user main_user;

#define number_of_potions 4

const int shadow_alpha=127;

/*colors
gray=0 (unallocated) {192, 192, 192, 255}
aqua blue=1 {51, 255, 255, 255}
green=2 {51, 255, 51, 255}
yellow=3 {255, 255, 51, 255}
pink=4 {255, 51, 255, 255}
violet=5 {153, 51, 255, 255}
gray=6 (no player) {160, 160, 160, 255}
background color=7 {220, 220, 220, 255}
*/

enum color_name{unallocated_color, blue, green, yellow, pink, violet, no_player_color, background_color, potion_on_screen};

enum potion_names{increase_soldiers_movement_speed, decrease_enemy_soldiers_movement_speed, enemy_soldiers_change_side, increase_soldiers_generation_speed};

SDL_Color potion_colors[number_of_potions]=
{
    {0, 0, 0, 255},
    {255, 255, 255, 255},
    {0, 0, 255, 255},
    {255, 0, 0, 255}
};

SDL_Color colors[number_of_colors]=
{
    {192, 192, 192, 192},
    {102, 255, 255, 255},
    {102, 255, 102, 255},
    {255, 255, 102, 255},
    {255, 102, 255, 255},
    {178, 102, 255, 255},
    {160, 160, 160, 255},
    {240, 240, 240, 255}
};

SDL_Color dark_colors[number_of_colors]=
{
    {},
    {0, 153, 153, 255},
    {0, 153, 0, 255},
    {153, 153, 0, 255},
    {153, 0, 153, 255},
    {76, 0, 153, 255},
    {255, 255, 255, 255},
    {}
};

enum logic_return_type{game_quit, previous_page, nothing, game_finished};