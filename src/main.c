#include <stdio.h>
#include "../lib/engine_Conway.h"

const int WINDOW_W = 10;
const int WINDOW_H = 10;

int main(int argv, char *argc[])
{
    if(!engine_Conway(WINDOW_W, WINDOW_H))
    {
        printf("ERROR: SOMETHING WRONG WITH CONWAY'S ENGINE");
    }
    return 0;
}

