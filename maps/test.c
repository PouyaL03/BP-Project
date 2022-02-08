#include <stdio.h>

int main()
{
    FILE* number_of_maps_file = fopen("number_of_maps.dat", "r");
    int number_of_maps;
    fread(&number_of_maps, sizeof(int), 1, number_of_maps_file);
    printf("%d\n", number_of_maps);
    fclose(number_of_maps_file);
    return 0;
}