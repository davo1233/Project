// Minesweeper game based on the Minesweeper game from Windows XP
// minesweeper.c
//
//

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE              8

// The possible command codes.
#define DETECT_ROW      1
#define DETECT_SQUARE   2
#define REVEAL_CROSS    3
#define GAME_MODE       4
#define FLAG_MINE       5
#define DEFUSE          6


// Add any extra #defines here.
#define OUT_OF_BOUNDS 7
#define OUT_OF_BOUNDS_RIGHT 7
#define OUT_OF_BOUNDS_DOWN 7
#define OUT_OF_BOUNDS_LEFT 0
#define OUT_OF_BOUNDS_UP 0
#define NOT_OUTSIDE 0
#define OUTSIDE 1
#define CROSS_SIZE 3
#define GAME_OVER 1
#define NOT_GAME_OVER 0
#define HINT_LIMIT 3
#define DEBUG 1
#define GAMEPLAY 0
void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void print_minefield(int minefield[SIZE][SIZE]);
void set_hidden_mines(int minefield[SIZE][SIZE], int no_of_mines);
void check_mine_each_row(int row, int col,int length, int minefield[SIZE][SIZE]);
int check_mine_entire_square(int row, int col,int size,int minefield[SIZE][SIZE]);
int reveal_cross(int row, int col, int minefield[SIZE][SIZE]);
void print_final_minefield(int minefield[SIZE][SIZE]);

int main(void) {
    int minefield[SIZE][SIZE];

    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    int no_of_mines = 0;
    printf("How many mines? ");
    scanf("%d",&no_of_mines);

    printf("Enter pairs:\n");
    set_hidden_mines(minefield,no_of_mines);

    printf("Game Started\n");
    print_debug_minefield(minefield);

    // A game ends when the player wins, loses, or enters EOF (Ctrl+D).
    int command = 0;
    int row = 0;
    int col = 0;
    int length = 0;
    int hints = 1;
    int game_mode = DEBUG;
    //parses each command specific to each 
    while(scanf("%d",&command) != EOF) {
        int game_over_flag = NOT_GAME_OVER;
        
        if (hints > HINT_LIMIT && (command == DETECT_ROW || command == DETECT_SQUARE)) {
            printf("Help already used\n");
        }
        if (command == DETECT_ROW) {
            scanf("%d %d %d", &row,&col,&length);
            check_mine_each_row(row,col,length,minefield);
            hints++;
        } else if (command == DETECT_SQUARE) {
            scanf("%d %d %d", &row,&col,&length);
            int mine_count = check_mine_entire_square(row,col,length,minefield);
            printf("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", mine_count, row,col,length);
            hints++;
        } else if (command == REVEAL_CROSS) {
            scanf("%d %d", &row,&col);
            game_over_flag = reveal_cross(row,col,minefield);
        } else if (command == GAME_MODE) {
            if (game_mode == DEBUG) {
                printf("Gameplay mode activated\n");
                game_mode = GAMEPLAY;
            } else if (game_mode == GAMEPLAY) {
                printf("Debug mode activated\n");
                game_mode = DEBUG;
            }
        }

        if (game_over_flag == GAME_OVER) {
            printf("Game over\n");
            if (game_mode == DEBUG) {
                print_debug_minefield(minefield);
            } else if (game_mode == GAMEPLAY) {
                print_final_minefield(minefield);
            }
            break;
        }

        if (game_mode == DEBUG) {
            print_debug_minefield(minefield);
        } else if (game_mode == GAMEPLAY) {
            print_minefield(minefield);
        }  
    }
    return 0;
}

void check_mine_each_row(int row, int col,int length, int minefield[SIZE][SIZE]) {
    int end_search_coord = col + length - 1;
    int mine_count = 0;
    int j = 0;
    int end = col + length;
    int out_of_bounds = NOT_OUTSIDE;
    while (j < end) {
        if (minefield[row][j] == HIDDEN_MINE) {
            mine_count++;
        } if (end_search_coord > OUT_OF_BOUNDS) {
            out_of_bounds = OUTSIDE;
            printf("Coordinates not on map\n");
            break;
        }
        j++;
    }
    if (out_of_bounds == NOT_OUTSIDE) {
        printf("There are %d mine(s) in row %d, from column %d to %d.\n",mine_count,row, col,end_search_coord);
    } 
}

int check_mine_entire_square(int row, int col,int size,int minefield[SIZE][SIZE]) {
    int outside_minefield = NOT_OUTSIDE;
    if (row > OUT_OF_BOUNDS_RIGHT || col > OUT_OF_BOUNDS_RIGHT || row < OUT_OF_BOUNDS_LEFT || col < OUT_OF_BOUNDS_LEFT) {
        printf("Coordinates not on map\n");
        outside_minefield = OUTSIDE;
    }
    int mine_count = 0;
    if (outside_minefield == NOT_OUTSIDE) {
        int starting_row = row - size/2;
        int finishing_row = row + size/2;
        
        while (starting_row <= finishing_row) {
            int starting_col = col - size/2;
            int finishing_col = col + size/2;
            while (starting_col <= finishing_col) {
                if (minefield[starting_row][starting_col] == HIDDEN_MINE) {
                    mine_count++;
                }
                starting_col++; 
            }
            starting_row++;
        }
        
    }
    return mine_count;
}

//when inputting the coordinates, c
int reveal_cross(int row, int col, int minefield[SIZE][SIZE]) { 
    int game_over_flag = NOT_GAME_OVER;  
    if (minefield[row][col] == HIDDEN_MINE) {
        game_over_flag = GAME_OVER;
        return game_over_flag;
    }

    if (row >= OUT_OF_BOUNDS_UP && row < OUT_OF_BOUNDS_DOWN) {
        if (check_mine_entire_square(row+1,col,CROSS_SIZE,minefield) == 0) {
            minefield[row+1][col] = VISIBLE_SAFE;
        }
    }

    if (row > OUT_OF_BOUNDS_UP && row <= OUT_OF_BOUNDS_DOWN) {
        if (check_mine_entire_square(row-1,col,CROSS_SIZE,minefield) == 0) {
            minefield[row-1][col] = VISIBLE_SAFE;
        }
    }

    if (col >= OUT_OF_BOUNDS_LEFT && col < OUT_OF_BOUNDS_RIGHT) {
        if (check_mine_entire_square(row,col+1,CROSS_SIZE,minefield) == 0) {
            minefield[row][col+1] = VISIBLE_SAFE;
        }
    }

    if (col > OUT_OF_BOUNDS_LEFT && col <= OUT_OF_BOUNDS_RIGHT) {
        if (check_mine_entire_square(row,col-1,CROSS_SIZE,minefield) == 0) {
            minefield[row][col-1] = VISIBLE_SAFE;
        } 
    }    
    minefield[row][col] = VISIBLE_SAFE;
    return game_over_flag;
}
//set mines in the minefield
void set_hidden_mines(int minefield[SIZE][SIZE], int no_of_mines) {
    int x = 0;
    int y = 0;
    for (int i = 0; i < no_of_mines;i++) {
        scanf("%d %d", &x, &y);
        if (x != SIZE || y != SIZE) {
            minefield[x][y] = HIDDEN_MINE;
        }
    }
}
// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            minefield[i][j] = HIDDEN_SAFE;
            j++;
        }
        i++;
    }
}

// Print out the actual values of the minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

void print_minefield(int minefield[SIZE][SIZE]) {
    printf("..\n");
    printf("\\/\n");
    
    int num_col = 0;
    printf("    ");
    while (num_col < SIZE) {
        printf("0%d ", num_col);
        num_col++;
    }
    printf("\n");
    printf("   -------------------------\n");
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        printf("0%d |",i);
        while (j < SIZE) {
            int num_of_mines = check_mine_entire_square(i,j,CROSS_SIZE,minefield);
            if (minefield[i][j] == HIDDEN_MINE || minefield[i][j] == HIDDEN_SAFE) {
                printf("##");
            } else if (num_of_mines > 0 && minefield[i][j] == VISIBLE_SAFE) {
                printf("0%d",num_of_mines);
            } else if (num_of_mines == 0 && minefield[i][j] == VISIBLE_SAFE) {
                printf("  ");
            }
            
            if (j < SIZE - 1) {
                printf(" ");
            } 
            j++;
        }
        printf("|\n");
        i++;
    }
}

void print_final_minefield(int minefield[SIZE][SIZE]) {
    printf("xx\n");
    printf("/\\\n");

    int num_col = 0;
    printf("    ");
    while (num_col < SIZE) {
        printf("0%d ", num_col);
        num_col++;
    }
    printf("\n");
    printf("   -------------------------\n");
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        printf("0%d |",i);
        while (j < SIZE) {
            int num_of_mines = check_mine_entire_square(i,j,CROSS_SIZE,minefield);
            if (minefield[i][j] == HIDDEN_SAFE) {
                printf("##");
            } else if (minefield[i][j] == HIDDEN_MINE) {
                printf("()");
            }else if (num_of_mines > 0 && minefield[i][j] == VISIBLE_SAFE) {
                printf("0%d",num_of_mines);
            } else if (num_of_mines == 0 && minefield[i][j] == VISIBLE_SAFE) {
                printf("  ");
            }
            
            if (j < SIZE - 1) {
                printf(" ");
            } 
            j++;
        }
        printf("|\n");
        i++;
    }
}