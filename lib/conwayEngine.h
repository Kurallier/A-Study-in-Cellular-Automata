#ifndef CONWAY_ENGINE_H
#define CONWAY_ENGINE_H

#include <SDL2/SDL_rect.h>
#include <stdint.h>

typedef enum cellState
{
    CELL_DEAD,
    CELL_ALIVE
}cellState;

typedef struct Automata
{
    //SDL_Point cellPoint;
    int pos_X, pos_Y;
    //R-G-B-A color values
    uint8_t r,g,b,a;
    cellState state;

}Automata;

Automata** conway_Automata_Matrix_Init(int canvW, int canvH, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

int conway_Automata_Matrix_Destroy(Automata** matrix, int row, int col);

int conway_Automata_Matrix_Modify(Automata** matrix, int oldRow, int oldCol, int newRows, int newCols);

int conway_Automata_Matrix_Seed(Automata** matrix, int row, int col, int num);

#endif
