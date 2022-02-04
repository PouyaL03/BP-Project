#include "helping-functions.h"

/*function prototypes*/
void draw_map(SDL_Renderer*, country all_countries[number_of_countries], TTF_Font* font);
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
            all_countries[i][j].glow_flag=0;
            all_countries[i][j].color=which_color;
            all_countries[i][j].x_center=space_from_side+initial_side_length*(1+1.5*j);
            if (j%2==0) all_countries[i][j].y_center=space_from_above+image_of_hexagon*i+initial_side_length*sin(teta);
            else all_countries[i][j].y_center=space_from_above+image_of_hexagon*(i+1);
            if (all_countries[i][j].color!=unallocated_color && all_countries[i][j].color != no_player_color)
            {
                all_countries[i][j].number_of_soldiers=20+rand()%5;
            }
            else all_countries[i][j].number_of_soldiers=10+rand()%4;
        }
    }
}

void draw_map(SDL_Renderer* renderer, country all_countries[number_of_countries], TTF_Font* font)
{
    for (int i=0 ; i<number_of_countries ; i++)
    {
        Uint8 alpha=255;
        int which_color=all_countries[i].color;
        draw_hexagon_borderline(renderer, all_countries[i].x_center,
                                all_countries[i].y_center,
                                initial_side_length,
                                all_countries[i].color,
                                alpha);
        if (which_color!=unallocated_color)
        {
            filledCircleRGBA(renderer, all_countries[i].x_center,
                                    all_countries[i].y_center,
                                    radius_of_circle,
                                    dark_colors[which_color].r,
                                    dark_colors[which_color].g,
                                    dark_colors[which_color].b,
                                    alpha);
            aacircleRGBA(renderer, all_countries[i].x_center, all_countries[i].y_center, radius_of_circle, 0, 0, 0, 255);
        }
        int number_of_soldiers=all_countries[i].number_of_soldiers;
        int x_corner_rect=all_countries[i].x_center;
        int y_corner_rect=all_countries[i].y_center+radius_of_circle;
        if(all_countries[i].color!=unallocated_color) show_number_of_soldiers(renderer, x_corner_rect, y_corner_rect, number_of_soldiers, font);
    }
    for (int i=0 ; i<number_of_countries ; i++)
    {
        Uint8 alpha=130;
        int which_color=all_countries[i].color;
        if (all_countries[i].glow_flag)
        {
            draw_hexagon_no_borderline(renderer, all_countries[i].x_center,
                                              all_countries[i].y_center,
                                              1.2*initial_side_length,
                                              which_color, alpha);
        }
    }
}
