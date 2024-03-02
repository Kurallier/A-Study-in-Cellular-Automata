#include <SDL2/SDL.h>
#include <stdlib.h>
#include "../lib/conwayEngine.h"

Automata** internal_Conway_Automata_Matrix_Create(int row, int col)
{
    Automata** matrix;

    matrix = malloc(sizeof(Automata*) * row);
    
    for(size_t i = 0; i < row; i++)
        matrix[i] = malloc(sizeof(Automata) * col);

    return matrix;
}

int internal_Conway_Automata_Matrix_Fill(int Mrow, int Mcol, Automata** cellMatrix, int r, int g, int b, int a)
{
    for(size_t i = 0; i < Mrow; i++)
    {
        for(size_t j = 0; j < Mcol; j++)
        {
            //cellMatrix[i][j].cellPoint.x = i;
            //cellMatrix[i][j].cellPoint.y = j;

            cellMatrix[i][j].pos_X = i;
            cellMatrix[i][j].pos_Y = j;

            cellMatrix[i][j].r = r;
            cellMatrix[i][j].g = g;
            cellMatrix[i][j].b = b;
            cellMatrix[i][j].a = a;

            cellMatrix[i][j].state = CELL_DEAD;
            
        }
    }
    return 0;
}

/*
 *\Modifies the 2d array
 *\Takes the old rows and coloums, and the quanity to be added
 *\NOT TOTAL ROWS/COL
 *\ONLY THE AMOUNT TO BE ADDED
 */
int conway_Automata_Matrix_Modify(Automata** matrix, int oldRow, int oldCol, int newRows, int newCols)
{
    int totalRow = (oldRow + newRows);
    int totalCol = (oldCol + newCols);

    if((newCols < 1) && (newRows > 1))
    {
        matrix = realloc(matrix, sizeof(Automata*) * totalRow);

        for(size_t j = totalRow - newRows; j < totalRow; j++)
            matrix[j] = malloc(sizeof(Automata) * oldCol);

        return 0;
    }

    else if((newCols > 1) && (newRows < 1))
    {
        for(size_t i = 0; i < oldRow; i++)
            matrix[i] = realloc(matrix[i], sizeof(Automata) * newCols);       
        return 0;
    }

    else if((newRows > 1) && (newCols > 1))
    {
        for(size_t i = 0; i < oldRow; i++)
            matrix[i] = realloc(matrix[i], sizeof(Automata) * newCols);       
        //
        matrix = realloc(matrix, sizeof(Automata*) * totalRow);

        for(size_t j = totalRow - newRows; j < totalRow; j++)
            matrix[j] = malloc(sizeof(Automata) * totalCol);
        
        return 0;
    }
    else
        return 1;
}

/*
 * \Frees up the memory of the 2d Automata array
 */
int conway_Automata_Matrix_Destroy(Automata** matrix, int row, int col)
{
    for(size_t i = 0; i < row; i++)
        free(matrix[i]);
    free(matrix);

    return 0;
}

//TODO: Allow for only a certain amount of points to be made; i.e. wont rand() the whole array
int conway_Automata_Matrix_Seed(Automata** matrix, int row, int col, int num)
{
   for(size_t i = 0; i < row; i++)
   {
       for(size_t j = 0; j < col; j++)
       {
           matrix[i][j].state = (rand() % 3);
       }
   }
   return 0;
}

/*
 * \Ceates an array of the cells
 * \The Canvas W and H
 * \Aswell as what color the pixel should be
 * \Sets each pixel's X an Y pos, and sets the cell state to CELL_DEAD
 */
Automata** conway_Automata_Matrix_Init(int canvW, int canvH, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    //B[i][j] = *(B[i]+j) = *(*(B+i)+j)
    Automata** cellMatrix = internal_Conway_Automata_Matrix_Create(canvW, canvH);

    internal_Conway_Automata_Matrix_Fill(canvW, canvH, cellMatrix, r, g, b, a);

    return cellMatrix;
}
