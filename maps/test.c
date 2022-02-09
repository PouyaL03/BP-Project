#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define number_of_hexagons_in_row 7
#define number_of_hexagons_in_column 5
#define window_width 1280
#define window_height 960
int number_of_countries=0;
double initial_side_length=70;
#define pi M_PI
#define teta pi/3

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

enum color_name{unallocated_color, blue, green, yellow, pink, violet, no_player_color, background_color};

void create_random_map(country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row]);
void initialize_country_array(country*,
                              country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row]);

int main()
{
    country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row];
    create_random_map(all_countries);
    country country_array[number_of_countries];
    initialize_country_array(country_array, all_countries);
    FILE* map_file = fopen("map_data/MAP0.dat", "w");
    fwrite(country_array, sizeof(country), number_of_countries, map_file);
    fclose(map_file);
    return 0;
}

void create_random_map(country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row])
{
    time_t t;
    srand(time(&t));
    int number_of_player_countries=0;
    int tmp=1;
    double space_from_above;
    double space_from_side;
    double image_of_hexagon=initial_side_length*2*sin(teta);
    space_from_above=(window_height-(number_of_hexagons_in_column+0.5)*image_of_hexagon)/2;
    space_from_side=(window_width-(number_of_hexagons_in_row*(initial_side_length*(1+cos(teta)))+initial_side_length*cos(teta)))/2;
    for (int i=0 ; i<number_of_hexagons_in_column ; i++)
    {
        for (int j=0 ; j<number_of_hexagons_in_row ; j++)
        {
            int which_color;
            if (tmp && rand()%20==0)
            {
                which_color=pink;
                tmp=0;
            }
            else if (rand()%2==0 && number_of_player_countries<12)
            {
                which_color=blue;
                number_of_player_countries++;
            }
            else which_color=0;
            all_countries[i][j].soldiers_in_use=0;
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
            number_of_countries++;
        }
    }
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