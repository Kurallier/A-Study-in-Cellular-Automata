//TODO:
//Figure out how to pass the arrays to the functions
#include <stdio.h>
#include "../lib/engine_Conway.h"

#define CELL_DEAD '-'
#define CELL_ALIVE '@'

//Create an enum to hold the state of the cells_grid
typedef enum State 
{
    DEAD,
    ALIVE,
} State;

//A structure that defines a single cell, it contains a double buffer for each cell and it's state
typedef struct cell 
{
    //Will be used as a double buffer for cell states
    State cells_bf_1, cells_bf_2;
} cells;

int func_init_cells();

//This will be used to set the state of all of the cells_grid to dead
int func_wipe_cells(size_t, size_t, cells* ptrcell[][]);

//Prints the current the grid of cells in buffer 1
int func_print_cells(size_t, size_t, cells* ptrcell[][]);


int engine_Conway(size_t grd_w, size_t grd_h)
{
    //Make the cells_grid into a grid(2d array)
    cells cells_grid[grd_w][grd_h];

    //Passing the adress to the array of cells to these fucntions
    if(!func_wipe_cells(grd_w, grd_h, &cells_grid[grd_w][grd_h]))
    {
        printf("ERROR: COULD NOT INIT CELLS");
    }

    //Just to test printing 
    cells_grid[grd_w/2][grd_h/2].cells_bf_1 = ALIVE;

    //Prints all of the cells in buffer 1
    if(!func_print_cells(grd_w, grd_h, &cells_grid[grd_w][grd_h]))
    {
        printf("ERROR: COULD NOT PRINT CELLS");
    }

    return 1;
}

int func_init_cells()
{

    return 1;
}

int func_wipe_cells(size_t grd_w, size_t grd_h, cells* ptrcell[grd_w][grd_h])
{
    for(size_t i = 0; i < grd_w; i++)
    {
        for(size_t j = 0; j < grd_h; j++)
        {
            ptrcell[i][j]->cells_bf_1 = DEAD;
        }
    }
    return 1;
}


int func_print_cells(size_t grd_w, size_t grd_h, struct cell* ptrcell[grd_w][grd_h])
{
    for(size_t i = 0; i < grd_w; i++)
    {
        for(size_t j = 0; j < grd_h; j++)
        {
            switch(ptrcell[i][j]->cells_bf_1)
            {
                case DEAD:
                    printf("%c", CELL_DEAD);
                    break;
                case ALIVE:
                    printf("%c", CELL_ALIVE);
                    break;
            }
            ptrcell++;
        }
        printf("\n");
    }

    return 1;
}
