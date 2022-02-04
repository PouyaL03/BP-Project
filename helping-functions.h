#include "constants.h"

/*function prototypes*/
void draw_hexagon_borderline(SDL_Renderer*, double, double, double, int, Uint8);
void draw_hexagon_no_borderline(SDL_Renderer*, double, double, double, int, Uint8);
void initialize_country_array(country country_array[number_of_hexagons_in_column*number_of_hexagons_in_row],
                              country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row]);
void check_mouse_state(SDL_Point, country*, double);
void show_number_of_soldiers(SDL_Renderer*, int, int, int, TTF_Font*);

void show_number_of_soldiers(SDL_Renderer* renderer, int x_corner, int y_corner, int number_of_soldiers, TTF_Font* font)
{
    int tmp_number_of_soldiers=number_of_soldiers;
    int check_for_positive_value=0;
    if (number_of_soldiers<0)
    {
        printf("number of players is lower than 0\n");
        check_for_positive_value=1;
    }
    SDL_Rect rectangle;
    SDL_Texture* texture;
    SDL_Color textColor={0, 0, 0, 255};
    /*turning number of players to a char* */
    char number_text[4];
    int length_of_number;
    for (length_of_number=0 ; tmp_number_of_soldiers>0 ; length_of_number++)
    {
        tmp_number_of_soldiers/=10;
    }
    number_text[3]='\0';
    if (length_of_number==0)
    {
        number_text[2]='0';
        length_of_number=1;
    }
    else
    {
        for (int i=0 ; i<length_of_number ; i++)
        {
            number_text[2-i]=number_of_soldiers%10+'0';
            number_of_soldiers/=10;
        }
    }
    if(check_for_positive_value==1)
    {
        length_of_number=3;
        number_text[0]='-';
        number_text[1]='-';
        number_text[2]='-';
    }
    SDL_Surface* surface;
    surface=TTF_RenderText_Solid(font, number_text+3-length_of_number, textColor);
    texture=SDL_CreateTextureFromSurface(renderer, surface);
    int text_width = surface->w;
    int text_height = surface->h;
    SDL_FreeSurface(surface);
    rectangle.x = x_corner-text_width/2;
    rectangle.y = y_corner;
    rectangle.w = text_width;
    rectangle.h = text_height;
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
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

void initialize_country_array(country country_array[number_of_hexagons_in_column*number_of_hexagons_in_row],
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