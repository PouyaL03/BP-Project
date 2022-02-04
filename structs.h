#include "include.h"
typedef struct soldier
{

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
    int efficiency;
}
country;