#include "helping-functions.h"

/*function prototypes*/
void draw_map(SDL_Renderer*, country*, TTF_Font*, TTF_Font*, attack**, potion*);
void create_random_map(country all_countries[number_of_hexagons_in_column][number_of_hexagons_in_row]);
void show_number_of_soldiers(SDL_Renderer*, int, int, int, TTF_Font*, country*, int);
void show_line(SDL_Renderer*, TTF_Font*);
void show_attacking_soldiers(SDL_Renderer*, country*, attack**);
void draw_hexagon_borderline(SDL_Renderer*, double, double, double, int, Uint8);
void draw_hexagon_no_borderline(SDL_Renderer*, double, double, double, int, Uint8);
void draw_potion(SDL_Renderer*, double, double, Uint8, int, double);

void draw_potion(SDL_Renderer* renderer, double x_center, double y_center, Uint8 alpha, int which_color, double side_length)
{
    Sint16 vx[6];
    Sint16 vy[6];
    vx[0]=x_center-side_length*cos(teta); vy[0]=y_center+side_length*sin(teta);
    vx[1]=x_center+side_length*cos(teta); vy[1]=y_center+side_length*sin(teta);
    vx[2]=x_center+side_length; vy[2]=y_center;
    vx[3]=x_center+side_length*cos(teta); vy[3]=y_center-side_length*sin(teta);
    vx[4]=x_center-side_length*cos(teta); vy[4]=y_center-side_length*sin(teta);
    vx[5]=x_center-side_length; vy[5]=y_center;
    filledPolygonRGBA(renderer, vx, vy, 6, potion_colors[which_color].r,
                                           potion_colors[which_color].g,
                                           potion_colors[which_color].b,
                                           alpha);
    aapolygonRGBA(renderer, vx, vy, 6, 0, 0, 0, alpha);
}

void show_attacking_soldiers(SDL_Renderer* renderer, country* country_array, attack** attack_head)
{
    int alpha=255;
    attack* temp_attack_head=*attack_head;
    for ( ; temp_attack_head!=NULL; temp_attack_head=temp_attack_head->next_attack)
    {
        if (temp_attack_head->attack_complete==0) continue;
        soldier* temp_soldier_head=temp_attack_head->soldier_head;
        while (temp_soldier_head!=NULL)
        {
            if (!(country_array[temp_attack_head->attacking_country_index].x_center-temp_soldier_head->soldier_position_x<5 &&
                country_array[temp_attack_head->attacking_country_index].y_center-temp_soldier_head->soldier_position_y<5 &&
                country_array[temp_attack_head->attacking_country_index].x_center-temp_soldier_head->soldier_position_x>-5 &&
                country_array[temp_attack_head->attacking_country_index].y_center-temp_soldier_head->soldier_position_y>-5))
                {
                    if ((country_array[temp_attack_head->defenfing_country_index].x_center-temp_soldier_head->soldier_position_x)*(country_array[temp_attack_head->defenfing_country_index].x_center-temp_soldier_head->soldier_position_x)+
                        (country_array[temp_attack_head->defenfing_country_index].y_center-temp_soldier_head->soldier_position_y)*(country_array[temp_attack_head->defenfing_country_index].y_center-temp_soldier_head->soldier_position_y)>
                        (radius_of_circle)*(radius_of_circle))
                          filledCircleRGBA(renderer, temp_soldier_head->soldier_position_x,
                                               temp_soldier_head->soldier_position_y,
                                               radius_of_circle/2,
                                               dark_colors[temp_soldier_head->color].r,
                                               dark_colors[temp_soldier_head->color].g,
                                               dark_colors[temp_soldier_head->color].b,
                                               alpha);
                }
            temp_soldier_head=temp_soldier_head->next_soldier;
        }
    }
}

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

void draw_map(SDL_Renderer* renderer, country* all_countries, TTF_Font* bold_font, TTF_Font* font, attack** attack_head, potion* all_colors_potion)
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
            if (all_colors_potion[which_color].enable==1)
            {
                for (int j=0 ; j<number_of_potions ; j++)
                {
                    if (all_colors_potion[which_color].type[j]==1)
                    {
                        draw_potion(renderer, all_countries[i].x_center,
                                              all_countries[i].y_center, 255, j, initial_side_length/2.8);
                    }
                }
            }
            else
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
        }
        int number_of_soldiers=all_countries[i].number_of_soldiers+all_countries[i].soldiers_in_use;
        int x_corner_rect=all_countries[i].x_center;
        int y_corner_rect=all_countries[i].y_center+radius_of_circle;
        if(all_countries[i].color!=unallocated_color) show_number_of_soldiers(renderer, x_corner_rect, y_corner_rect, number_of_soldiers, bold_font, all_countries, i);
    }
    /*for glowing*/
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
    if (all_colors_potion[potion_on_screen].enable==1)
    {
        for (int j=0 ; j<number_of_potions ; j++)
        {
            if (all_colors_potion[potion_on_screen].type[j]==1)
            {
                draw_potion(renderer, all_colors_potion[potion_on_screen].x_center,
                                      all_colors_potion[potion_on_screen].y_center, 255, j, initial_side_length/2);
            }
        }
    }
    show_attacking_soldiers(renderer, all_countries, attack_head);
    show_line(renderer, font);
}

void show_line(SDL_Renderer* renderer, TTF_Font* font)
{
    SDL_Rect rectangle;
    SDL_Texture* texture;
    SDL_Color textColor={0, 0, 0, 255};
    SDL_Surface* surface;
    if (waiting_for_attack==0) surface=TTF_RenderText_Solid(font, "please choose country to attack from", textColor);
    else if (waiting_for_attack==1) surface=TTF_RenderText_Solid(font, "plase choose where you want to attack", textColor);
    texture=SDL_CreateTextureFromSurface(renderer, surface);
    int text_width = surface->w;
    int text_height = surface->h;
    SDL_FreeSurface(surface);
    rectangle.x = (window_width-text_width)/2;
    rectangle.y = window_height-(text_height+10);
    rectangle.w = text_width;
    rectangle.h = text_height;
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
    SDL_DestroyTexture(texture);
}

void show_number_of_soldiers(SDL_Renderer* renderer, int x_corner, int y_corner, int number_of_soldiers, TTF_Font* font, country* country_array, int country_index)
{
    int tmp_number_of_soldiers=number_of_soldiers;
    int check_for_positive_value=0;
    if (number_of_soldiers<0)
    {
        if (country_array[country_index].number_of_soldiers<0)
        {
            country_array[country_index].number_of_soldiers=0;
        }
        if (country_array[country_index].soldiers_in_use<0)
        {
            country_array[country_index].soldiers_in_use=0;
        }
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
    SDL_DestroyTexture(texture);
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
