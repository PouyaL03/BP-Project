#include "constants.h"

/*function prototypes*/
void draw_hexagon_borderline(SDL_Renderer*, double, double, double, int, Uint8);
void draw_hexagon_no_borderline(SDL_Renderer*, double, double, double, int, Uint8);
void create_random_map(country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row]);

void create_random_map(country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row])
{
    time_t t;
    srand(time(&t));
    double space_from_above;
    double space_from_side;
    double image_of_hexagon=initial_side_length*2*sin(teta);
    space_from_above=(window_height-(number_of_hexagons_in_column+0.5)*image_of_hexagon)/2;
    space_from_side=(window_width-(number_of_hexagons_in_row*(initial_side_length*(1+cos(teta)))+initial_side_length*cos(teta)))/2;
    for (int i=0 ; i<number_of_hexagons_in_column ; i++)
    {
        for (int j=0 ; j<number_of_hexagons_in_row ; j++)
        {
            int which_color=rand()%10+1;
            if (which_color>6)
            {
                if (which_color>8) which_color=0;
                else which_color=6;
            }
            all_countries[i][j].color=which_color;
            all_countries[i][j].x_center=space_from_side+initial_side_length*(1+1.5*j);
            if (j%2==0) all_countries[i][j].y_center=space_from_above+image_of_hexagon*i+initial_side_length*sin(teta);
            else all_countries[i][j].y_center=space_from_above+image_of_hexagon*(i+1);
        }
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