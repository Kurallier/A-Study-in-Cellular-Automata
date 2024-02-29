//TODO: Figure out why math don't work
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ALIVE '@'
#define DEAD '-'

int row = 5;
int coll = 10;
int iterations = 10;

int wipe_grid(int grid[row][coll], int row, int coll);
int randomize_grid(int grid[row][coll], int row, int coll);
int count_alive(int grid[row][coll], int , int );
int gen_next(int grid[row][coll], int grid_b[row][coll], int row, int coll);
int print_grid(int grid[row][coll], int row, int coll);
int swap_grid(int grid[row][coll], int grid_b[row][coll], int row, int coll);

int print_Ascii(void);

int main(int argv, char* argc[])
{
    int gameIsRun = 1;
    char usrInput;

    print_Ascii();

    printf("Welcome to Conway's Game of Life\n");
    printf("\nPlease enter the number of rows: ");
    scanf("%d", &row);
    printf("\nPlease enter the number of columns: ");
    scanf("%d", &coll);
    printf("\nPlease enter the number of iterations: ");
    scanf("%d", &iterations);
    fgetc(stdin);
    
    int grid[row][coll];
    int grid_b[row][coll];

    //Wipes both grids
    wipe_grid(grid, row, coll);
    wipe_grid(grid_b, row, coll);

    //Rands cell in first grid
    randomize_grid(grid, row, coll);

    //For debug purposes
    /*grid[1][5] = 1;
    grid[2][4] = 1;
    grid[1][6] = 1;
    grid[2][6] = 1;
    grid[3][6] = 1;
    grid[3][6] = 1;*/

    print_grid(grid, row, coll);
    printf("\n");

    int i = 0;
    while(gameIsRun)
    {
        printf("\nEnter Y to continue to the next iteration");
        printf("\nEnter N to exit: ");
        scanf("\n%c", &usrInput);
        if(usrInput == 'Y' || usrInput == 'y')
        {
            printf("\nGenerating next iteration...");
            gen_next(grid, grid_b, row, coll);
        }
        else if(usrInput == 'N' || usrInput == 'n')
        {
            printf("\nAre you sure you want to exit? Y/N:");
            scanf("\n%c", &usrInput);
            if(usrInput == 'Y' || usrInput == 'y')
            {
                printf("\n Thanks for playing!");
                gameIsRun = 0;
            }
        }
        
        if(i == iterations)
        {
            printf("\n The game has ended, thanks for playing!");
            gameIsRun = 0;
        }
        i++;
    }
    /*for(i = 0; i < iterations; i++)
    {
        gen_next(grid, grid_b, row, coll);
    }*/
    
    return 0;
}

int print_Ascii(void)
{
    FILE* fp_ascii = NULL;
    char ascii_path[] = "../assets/ascii.txt", c;

    fp_ascii = fopen(ascii_path, "r");
    if(fp_ascii == NULL)
    {
        printf("Error opening ascii.txt");
        return 1;
    }
    c = fgetc(fp_ascii);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(fp_ascii);
    }
    fclose(fp_ascii);
}

int wipe_grid(int grid[row][coll], int row, int coll)
{
    for(size_t r = 0; r < row; r++)
    {
        for(size_t c = 0; c < coll; c++)
        {
            grid[r][c] = 0;
        }
    }
    return 1;
}

int randomize_grid(int grid[row][coll], int row, int coll)
{
    srand(time(NULL));
    for(size_t r = 0; r < row; r++)
    {
        for(size_t c = 0; c < coll; c++)
        {
            grid[r][c] = (rand() % 2);
        }
    }
    return 1;
}

int print_grid(int grid[row][coll], int row, int coll)
{
    for(size_t r = 0; r < row; r++)
    {
        for(size_t c = 0; c < coll; c++)
        {
            if(grid[r][c] == 1) 
            {
                printf("%c", ALIVE);
            }
            else 
            {
                printf("%c", DEAD);
            }

        }
        printf("\n");
    }
    return 1;
}

int count_alive(int grid[row][coll], int x, int y)
{
    int count = 0;
    for(int r = (x -1); r <= (x + 1); r++)
    {
        for(int c = (y-1); c <= (y+1); c++)
        {
            if((r==x && c==y) || (r<0 || c<0)) // || (r >= x || c >= y))
            { 
                continue;
            }
            if(grid[r][c]== 1)
            {
                count++;
            }
        }
    }
    return count;
}

int gen_next(int grid[row][coll], int grid_b[row][coll], int row, int coll)
{
    for(int rw = 0; rw < row; rw++)
    {
        for(int co = 0; co < coll; co++)
        {
            int live_cells = 0;
            live_cells = count_alive(grid, rw, co);
            //printf("%d", live_cells);

            if(((live_cells == 2) || (live_cells == 3)) && grid[rw][co] == 1)
            {
                grid_b[rw][co] = 1;
            }
            else if(grid[rw][co]==0 && live_cells==3)//change to ||
            {
                grid_b[rw][co]=1;
            }

            else
            {
                grid_b[rw][co]=0;
            }

        }
        printf("\n");
    }
    print_grid(grid_b, row, coll);
    swap_grid(grid, grid_b, row, coll);
    return 1;
}

int swap_grid(int grid[row][coll], int grid_b[row][coll], int row, int coll)
{
    wipe_grid(grid, row, coll);
    for(size_t i = 0; i < row; i++)
    {
        for( size_t j = 0; j < coll; j++)
        {
            grid[i][j] = grid_b[i][j];
        }
    }
    wipe_grid(grid_b, row, coll);

    return 1;
}

