// Assignment 1 21T2 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by YOUR-NAME-HERE (z5555555)
// on INSERT-DATE-HERE
//
// Version 1.0.0 (2021-06-19): Assignment released.

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
#define OUT_OF_BOUNDS_RIGHT 8
#define OUT_OF_BOUNDS_LEFT 0
#define NOT_OUTSIDE 0
#define OUTSIDE 1
void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void set_hidden_mines(int minefield[SIZE][SIZE], int no_of_mines);
void check_mine_each_row(int row, int col,int length, int minefield[SIZE][SIZE]);
void check_mine_entire_square(int row, int col,int size,int minefield[SIZE][SIZE]);

// Place your function prototyes here.

int main(void) {
    int minefield[SIZE][SIZE];

    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    int no_of_mines = 0;
    printf("How many mines? ");
    scanf("%d",&no_of_mines);

    // TODO: Scan in the number of pairs of mines.

    printf("Enter pairs:\n");

    // TODO: Scan in the pairs of mines and place them on the grid.
    set_hidden_mines(minefield,no_of_mines);

    printf("Game Started\n");
    print_debug_minefield(minefield);


    // TODO: Scan in commands to play the game until the game ends.
    // A game ends when the player wins, loses, or enters EOF (Ctrl+D).
    // You should display the minefield after each command has been processed.
    int command = 0;
    int row = 0;
    int col = 0;
    int length = 0;

    while(scanf("%d %d %d %d",&command,&row,&col,&length) != EOF) {
        if (command == DETECT_ROW) {
            check_mine_each_row(row,col,length,minefield);
        } else if (command == DETECT_SQUARE) {
            check_mine_entire_square(row,col,length,minefield);
        }
        print_debug_minefield(minefield);
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

void check_mine_entire_square(int row, int col,int size,int minefield[SIZE][SIZE]) {
    int outside_minefield = NOT_OUTSIDE;
    if (row > OUT_OF_BOUNDS_RIGHT || col > OUT_OF_BOUNDS_RIGHT || row < OUT_OF_BOUNDS_LEFT || col < OUT_OF_BOUNDS_LEFT) {
        printf("Coordinates not on map\n");
        outside_minefield = OUTSIDE;
    }

    if (outside_minefield == NOT_OUTSIDE) {
        int starting_row = row - size/2;
        int finishing_row = row + size/2;
        printf("starting row: %d finishing row: %d\n", starting_row, finishing_row);
        int mine_count = 0;
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
        printf("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", mine_count, row,col,size);
    }
    
}

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