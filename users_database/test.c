#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct user
{
    char username[21];
    int score;
}
user;

char* names[10]=
{
    "user1",
    "user2",
    "user3",
    "user4",
    "user5",
    "user6",
    "user7",
    "user8",
    "user9",
    "user10"
};

int main()
{
    srand(0);
    FILE* file=fopen("users.dat", "r");
    FILE* number_of_users_file=fopen("number_of_users.dat", "r");
    int number_of_users;
    fread(&number_of_users, sizeof(int), 1, number_of_users_file);
    printf("%d\n", number_of_users);
    user tmp[number_of_users];
    fread(tmp, sizeof(user), number_of_users, file);
    for (int i=0 ; i<number_of_users ; i++)
    {
        printf("'%s'\t%d\n", tmp[i].username, tmp[i].score);
    }

    // FILE* file=fopen("users.dat", "w");
    // user tmp[10];
    // for (int i=0 ; i<10 ; i++)
    // {
    //     tmp[i].score=(rand()%50);
    //     strcpy(tmp[i].username, names[i]);
    //     printf("%s\n", names[i]);
    // }
    // fwrite(tmp, sizeof(user), 10, file);

    // FILE* file=fopen("number_of_users.dat", "w");
    // int tmp=0;
    // fwrite(&tmp, sizeof(int), 1, file);

    // FILE* file=fopen("number_of_users.dat", "r");
    // int tmp;
    // fread(&tmp, sizeof(int), 1, file);
    // printf("%d\n", tmp);

    fclose(file);
    return 0;
}