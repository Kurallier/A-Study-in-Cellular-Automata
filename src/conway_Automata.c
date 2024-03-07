#include "../lib/conwayEngine.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_thread.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>

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
    //Store the amount of live cells in the object
    matrix[cellX][cellY].surrounding_live_cells = cell_Count;
    return 0;
}

int internal_Conway_Rules_Apply(int cellX, int cellY, Automata **matrix, int canvW, int canvH)
{
    internal_Conway_Rules_Count_Alive(matrix, cellX, cellY, canvW, canvH);

    if((matrix[cellX][cellY].surrounding_live_cells == 2)  && (matrix[cellX][cellY].state == CELL_ALIVE))
    {
        matrix[cellX][cellY].state = CELL_ALIVE;
        return 0;
    }
    else if(matrix[cellX][cellY].surrounding_live_cells == 3)
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
    memcpy(matrix_Buffer, matrix, canvW * canvH * sizeof(Automata *));

    for(int i = 0; i < canvW; i++)
    {
        for(int j = 0; j < canvH; j++)
        {
            internal_Conway_Rules_Apply(i, j, matrix_Buffer, canvW, canvH);
            //internal_Conway_Rules_Apply_Threaded(i, j, matrix_Buffer, canvW, canvH, &rulesMutex);
        }
    }
    //Swap buffers
    memcpy(matrix, matrix_Buffer, canvW * canvH * sizeof(Automata *));

    return 0;
}

// Define the struct for passing data to threads
typedef struct RulesApplyParams
{
    int start_row;
    int end_row;
    //int cellX, cellY;
    int canvW, canvH;
    Automata** array;
    SDL_mutex* mutex;
}RulesApplyParams;
/*
 * EXPERIMENTAL FUNCTION
 * 
 * Will act as a layer between the rules function and the main function
 * should help with performance
 */
int internal_Conway_Rules_Apply_Threaded(/*RulesApplyParams* RulesApplyData*/ void* RulesApplyData)
{
    RulesApplyParams* RulesApplyParam = RulesApplyData;

    SDL_LockMutex(RulesApplyParam->mutex);
    for (int i = RulesApplyParam->start_row; i <= RulesApplyParam->end_row; i++) 
    {
        for (int j = 0; j < RulesApplyParam->canvH; j++) 
        {

            internal_Conway_Rules_Count_Alive(RulesApplyParam->array, i, j, RulesApplyParam->canvW, RulesApplyParam->canvH);

            if((RulesApplyParam->array[i][j].surrounding_live_cells == 2)  && (RulesApplyParam->array[i][j].state == CELL_ALIVE))
            {
                RulesApplyParam->array[i][j].state = CELL_ALIVE;
            }
            else if(RulesApplyParam->array[i][j].surrounding_live_cells == 3)
            {
                RulesApplyParam->array[i][j].state = CELL_ALIVE;
            }
            else 
            {
                RulesApplyParam->array[i][j].state = CELL_DEAD;
            }

        }
    }
    SDL_UnlockMutex(RulesApplyParam->mutex);

    /*
    for(int i = 0; i < canvW; i++)
    {
        for(int j = 0; j < canvH; j++)
        {

            //pthread_mutex_lock(&rulesMutex);
            internal_Conway_Rules_Count_Alive(matrix, i, j, canvW, canvH);

        }
    }
    */
    return 0;
}

int conway_Generation_Next_Threaded(int canvW, int canvH, Automata **matrix, Automata**matrix_Buffer)
{
    /*
     * Could improve function by having the array be split up
     * and the rules function  be threaded to deal with each section of the array.
     */
    //Creates a double buffer for the results to be stored into
    memcpy(matrix_Buffer, matrix, canvW * canvH * sizeof(Automata *));
/*------------ Thread init ------------------------*/
    const int NUM_THREADS = 4;

    // Create mutex
    SDL_mutex* RulesMutex;
    RulesMutex = SDL_CreateMutex();
    if(!RulesMutex)
    {
        printf("Failed to create mutex: %s\n", SDL_GetError()); 
        return 1;
    }

    // Init threads
    SDL_Thread* threads[NUM_THREADS];
    RulesApplyParams *RulesApplyParam = malloc(NUM_THREADS * sizeof(RulesApplyParams));
    if (RulesApplyParam == NULL) 
    {
    // Handle memory allocation failure
        printf("Failed to malloc thread params");
        return 1;
    }

    int rows_per_thread = canvW / NUM_THREADS;

    //Create the threads and pass args 
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        //FIX: I suspect that the math used to calc the start and end row are causing graphical error
        RulesApplyParam[i].array = matrix_Buffer;
        RulesApplyParam[i].start_row = i * rows_per_thread;
        RulesApplyParam[i].end_row = ((i + 1) * rows_per_thread - 1);
        RulesApplyParam[i].mutex = RulesMutex;

        threads[i] = SDL_CreateThread(internal_Conway_Rules_Apply_Threaded, "Rule compute thread", &RulesApplyParam[i]);
        if (!threads[i]) 
        {
            printf("SDL thread creation failed: %s\n", SDL_GetError());
            SDL_DestroyMutex(RulesMutex);
            free(RulesApplyParam);
            return 1;
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        SDL_WaitThread(threads[i], NULL);
    }

    /*
    for(int i = 0; i < canvW; i++)
    {
        RulesApplyParams.cellX = i;
        for(int j = 0; j < canvH; j++)
        {
            RulesApplyParams.cellY = j;
            //internal_Conway_Rules_Apply_Threaded(i, j, matrix_Buffer, canvW, canvH, &rulesMutex);
            //internal_Conway_Rules_Apply(i, j, matrix_Buffer, canvW, canvH);
        }
    }
    */

    //Swap buffers
    memcpy(matrix, matrix_Buffer, canvW * canvH * sizeof(Automata *));

    // Destroy mutex
    SDL_DestroyMutex(RulesMutex);
    //Free array 
    free(RulesApplyParam);

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


