#include <stdio.h>
#include <stdlib.h>

typedef struct user {
    char name[24];
    char password[24];
} user_t;

user_t *read_users(const char *path, int *n)
{
    int i;
    user_t *db;
    FILE *f = fopen(path, "r");

    fscanf(f, "%d", n);
    db = malloc(sizeof(user_t) * (*n));

    for(i = 0; i < *n; i++) {
        fscanf(f, "%s %s", db[i].name, db[i].password);
    }

    return db;
}
