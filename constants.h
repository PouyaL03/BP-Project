#include "structs.h"
int window_width=1280;
int window_height=960;
int number_of_hexagons_in_row=8;
int number_of_hexagons_in_column=6;
double initial_side_length=70;
double radius_of_circle=20;
const int FPS=24;
#define number_of_colors 8
#define pi M_PI
#define teta pi/3

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

SDL_Color colors[number_of_colors]=
{
    {192, 192, 192, 192},
    {51, 255, 255, 255},
    {51, 255, 51, 255},
    {255, 255, 51, 255},
    {255, 51, 255, 255},
    {153, 51, 255, 255},
    {160, 160, 160, 255},
    {220, 220, 220, 255}
};

SDL_Color dark_colors[number_of_colors]=
{
    {},
    {0, 153, 153, 255},
    {0, 153, 0, 255},
    {153, 153, 0, 255},
    {153, 0, 153, 255},
    {76, 0, 153, 255},
    {96, 96, 96, 0},
    {}
};