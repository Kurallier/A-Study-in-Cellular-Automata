#include "../lib/conwayEngine.h"
#include <pthread.h>

/*------------------------- Cell Matrix---------------------------------------------------------------------------*/

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
 * \Possibly broken as of now
 */
int conway_Automata_Matrix_Destroy(Automata** matrix, int row, int col)
{
    for(size_t i = 0; i < row; i++)
        free(matrix[i]);
    free(matrix);

    return 0;
}

int conway_Automata_Matrix_Seed(Automata** matrix, int row, int col, int num)
{
    for(size_t k = 0; k < num; k++)
    {
        matrix[(rand() % row)][(rand() % col)].state = CELL_ALIVE;
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

/* ----------------- Game Rules ----------------------------------------------------------*/


int internal_Conway_Rules_Count_Alive(Automata** matrix, int cellX, int cellY, int canvW, int canvH)
{
    int cell_Count = 0;

    for(int i = (cellX - 1); i <= (cellX + 1) ; i++)
    {
        for(int j = (cellY - 1); j <= (cellY + 1); j++)
        {
            //If check is looking at the current cell; if the check is out of array; if the check is out of bounds
            if(((i == cellX && j == cellY) || (i < 0 || j < 0)) || (i > (canvW - 1) || j > (canvH - 1)))
                continue;
            if (matrix[i][j].state == CELL_ALIVE)
                cell_Count++;
        }
    }
    return cell_Count;
}

int internal_Conway_Rules_Apply(int cellX, int cellY, Automata **matrix, int canvW, int canvH)
{
    int live_Cells = internal_Conway_Rules_Count_Alive(matrix, cellX, cellY, canvW, canvH);
    if((live_Cells == 2)  && (matrix[cellX][cellY].state == CELL_ALIVE))
    {
        matrix[cellX][cellY].state = CELL_ALIVE;
        return 0;
    }
    else if(live_Cells == 3)
    {
        matrix[cellX][cellY].state = CELL_ALIVE;
        return 0;
    }
    else 
        {
            matrix[cellX][cellY].state = CELL_DEAD;
            return 0;
        }
}

/*
 * EXPERIMENTAL FUNCTION
 * 
 * Will act as a layer between the rules function and the main function
 * should help with performance
 *
void* internal_Conway_Rules_Apply_Threaded(int cellX, int cellY, Automata **matrix, int canvW, int canvH, pthread_mutex_t* rulesMutex)
{
    pthread_mutex_lock(rulesMutex);
    int live_Cells = internal_Conway_Rules_Count_Alive(matrix, cellX, cellY, canvW, canvH);
    if((live_Cells == 2)  && (matrix[cellX][cellY].state == CELL_ALIVE))
    {
        matrix[cellX][cellY].state = CELL_ALIVE;
        return 0;
    }
    else if(live_Cells == 3)
    {
        matrix[cellX][cellY].state = CELL_ALIVE;
        return 0;
    }
    else 
        {
            matrix[cellX][cellY].state = CELL_DEAD;
            return 0;
        }
    pthread_mutex_unlock;
}
*/



/*
 * \Iterates the current generation 
 * \Only logic, no rendering capabilities
 * \Takes the array, and it's dimensions 
 */ 
int conway_Generation_Next(int canvW, int canvH, Automata **matrix, Automata**matrix_Buffer)
{
    /*
     * Could improve function by having the array be split up
     * and the rules function  be threaded to deal with each section of the array.
     *
     * BIGGEST PERFORMANCE ISSUE IS THE RULES FUNCTION
     */

    /*
     * WILL DEAL WITH THIS LATER, MY BRAIN IS TOO DEAD TO DO THIS RN
    int numThreads = 1; 
    pthread_t Threads[numThreads];

    pthread_mutex_t rulesMutex;
    pthread_mutex_init(&rulesMutex, NULL);

    if(pthread_create(Threads[1], NULL, &internal_Conway_Rules_Apply_Threaded()) != 0)
    {
        printf("FAILED CREATING THREAD");
    }
    */

    //Creates a double buffer for the results to be stored into
    memcpy(&matrix_Buffer, &matrix, sizeof(matrix));

    for(int i = 0; i < canvW; i++)
    {
        for(int j = 0; j < canvH; j++)
        {
            internal_Conway_Rules_Apply(i, j, matrix_Buffer, canvW, canvH);
            //internal_Conway_Rules_Apply_Threaded(i, j, matrix_Buffer, canvW, canvH, &rulesMutex);
        }
    }
    //Swap buffers
    memcpy(&matrix, &matrix_Buffer, sizeof(matrix));
    return 0;
}

/*-------------------------------------------------------------------------------*/

//Random function to draw the matrix cuz I have no idea where to put it
//TODO: find a way to implement SDL_RenderDrawPoints
int SDL_Render_Emplace_Automata_Matrix(Automata** AutomataMatrix, int canvas_w, int canvas_h, SDL_Renderer* m_renderer, SDL_Texture* m_texture)
{
    SDL_SetRenderTarget(m_renderer, m_texture);
    for(size_t i = 0; i < canvas_w; i++)
    {
        for(size_t j = 0; j < canvas_h; j++)
        {
            if(AutomataMatrix[i][j].state == CELL_ALIVE)
            {
                SDL_SetRenderDrawColor(m_renderer, AutomataMatrix[i][j].r,AutomataMatrix[i][j].g,AutomataMatrix[i][j].b,AutomataMatrix[i][j].a);
                SDL_RenderDrawPoint(m_renderer, i, j);
            }
        }
    }
    return 0;
}


