#ifndef CONWAY_ENGINE_H
#define CONWAY_ENGINE_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>

typedef enum cellState
{
    CELL_DEAD = 0,
    CELL_ALIVE = 1,
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

int conway_Generation_Next(int canvW, int canvH, Automata** matrix);

int SDL_Render_Emplace_Automata_Matrix(Automata** AutomataMatrix, int canvas_w, int canvas_h, SDL_Renderer* m_renderer, SDL_Texture* m_texture);

#endif
