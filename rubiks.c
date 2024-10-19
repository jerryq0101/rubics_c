#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void enable_raw_mode();
void disable_raw_mode();
void move_cursor(int row, int col);
void update_char(int row, int col, char c);
void setup_cube(int cube_slab[12][9]);
void print_cube(int cube_slab[12][9]);
void rotate_cube(int axis, char direction);

#define EXIT_CHAR '~'
#define INPUT_ROW 13
#define INPUT_COL 1

#define BLOCK "\u2588"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

const char* COLORS[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};

/*
6 sides of a cube
*/
int cube_slab[12][9] = {{0}};

/*
9 rotation axis
*/
int *axis_1[12] = {*(cube_slab + 2)+3, *(cube_slab + 2)+4, *(cube_slab + 2)+5, *(cube_slab + 3)+6, *(cube_slab + 4)+6, *(cube_slab + 5)+6, *(cube_slab + 6)+5, *(cube_slab + 6)+4, *(cube_slab + 6)+3, *(cube_slab + 5)+2, *(cube_slab + 4)+2, *(cube_slab + 3)+2};
int *axis_2[12] = {*(cube_slab + 1)+3, *(cube_slab + 1)+4, *(cube_slab + 1)+5, *(cube_slab + 3)+7, *(cube_slab + 4)+7, *(cube_slab + 5)+7, *(cube_slab + 7)+5, *(cube_slab + 7)+4, *(cube_slab + 7)+3, *(cube_slab + 5)+1, *(cube_slab + 4)+1, *(cube_slab + 3)+1};
int *axis_3[12] = {*(cube_slab + 0)+3, *(cube_slab + 0)+4, *(cube_slab + 0)+5, *(cube_slab + 3)+8, *(cube_slab + 4)+8, *(cube_slab + 5)+8, *(cube_slab + 8)+5, *(cube_slab + 8)+4, *(cube_slab + 8)+3, *(cube_slab + 5)+0, *(cube_slab + 4)+0, *(cube_slab + 3)+0};
int *axis_4[12] = {*(cube_slab)+3, *(cube_slab + 1)+3, *(cube_slab + 2)+3, *(cube_slab + 3)+3, *(cube_slab + 4)+3, *(cube_slab + 5)+3, *(cube_slab + 6)+3, *(cube_slab + 7)+3, *(cube_slab + 8)+3, *(cube_slab + 9)+3, *(cube_slab + 10)+3, *(cube_slab + 11)+3};
int *axis_5[12] = {*(cube_slab)+4, *(cube_slab + 1)+4, *(cube_slab + 2)+4, *(cube_slab + 3)+4, *(cube_slab + 4)+4, *(cube_slab + 5)+4, *(cube_slab + 6)+4, *(cube_slab + 7)+4, *(cube_slab + 8)+4, *(cube_slab + 9)+4, *(cube_slab + 10)+4, *(cube_slab + 11)+4};
int *axis_6[12] = {*(cube_slab)+5, *(cube_slab + 1)+5, *(cube_slab + 2)+5, *(cube_slab + 3)+5, *(cube_slab + 4)+5, *(cube_slab + 5)+5, *(cube_slab + 6)+5, *(cube_slab + 7)+5, *(cube_slab + 8)+5, *(cube_slab + 9)+5, *(cube_slab + 10)+5, *(cube_slab + 11)+5};
int *axis_7[12] = {*(cube_slab+3)+0, *(cube_slab+3)+1, *(cube_slab + 3)+2, *(cube_slab + 3)+3, *(cube_slab + 3)+4, *(cube_slab + 3)+5, *(cube_slab + 3)+6, *(cube_slab + 3)+7, *(cube_slab + 3)+8, *(cube_slab + 9)+5, *(cube_slab + 9)+4, *(cube_slab + 9)+3};
int *axis_8[12] = {*(cube_slab+4)+0, *(cube_slab+4)+1, *(cube_slab + 4)+2, *(cube_slab + 4)+3, *(cube_slab + 4)+4, *(cube_slab + 4)+5, *(cube_slab + 4)+6, *(cube_slab + 4)+7, *(cube_slab + 4)+8, *(cube_slab + 10)+5, *(cube_slab + 10)+4, *(cube_slab + 10)+3};
int *axis_9[12] = {*(cube_slab+5)+0, *(cube_slab+5)+1, *(cube_slab + 5)+2, *(cube_slab + 5)+3, *(cube_slab + 5)+4, *(cube_slab + 5)+5, *(cube_slab + 5)+6, *(cube_slab + 5)+7, *(cube_slab + 5)+8, *(cube_slab + 11)+5, *(cube_slab + 11)+4, *(cube_slab + 11)+3};;



struct termios orig_termios;


int main(void) {
        enable_raw_mode();
        printf("\033[2J\033[H");

        setup_cube(cube_slab);
        printf("\033[2J\033[H");
        print_cube(cube_slab);

        // Ready for input
        move_cursor(INPUT_ROW, INPUT_COL);

        int c;
        int count = 0;
        static int axis = 1;

        while ((c = getchar()) != EXIT_CHAR) {
                switch (c) {
                        case 'w':
                                axis = axis == 10 ? 1 : axis + 1;
                                break;
                        case 's':
                                axis = axis == 0 ? 9 : axis - 1;
                                break;
                        case 'r':
                                rotate_cube(axis, 'r');
                                break;
                        case 'l':
                                rotate_cube(axis, 'l');
                                break;
                        case 'u':
                                rotate_cube(axis, 'u');
                                break;
                        case 'd':
                                rotate_cube(axis, 'd');
                                break;
                }

                printf("\033[2J\033[H");
                print_cube(cube_slab);

                // Ready for input
                move_cursor(INPUT_ROW, INPUT_COL);
        }
        
        disable_raw_mode();
        return 0;
}



// Array processing --------------------

void shift_array_3(int *array[], int direction);
void print_block(int start_row, int start_col);

// Refactor this to pointer operations afterwards to flex 
void setup_cube(int cube_slab[12][9]) {
        // load first cube
        for (int j = 3; j < 6; j++)  {
                for (int i = 0; i < 3; i++) {
                        while ((cube_slab[i][j] = rand() % 7) == 0)
                                ;
                }
        }

        for (int j = 3; j < 6; j++) {
                for (int i = 3; i < 12; i++) {
                        while ((cube_slab[i][j] = rand() % 7) == 0)
                                ;
                }
        }
        
        // load rows
        for (int i = 3; i < 6; i++) {
                for (int j = 0; j < 9; j++) {
                        while ((cube_slab[i][j] = rand() % 7) == 0)
                                ;
                }
        }
}

void print_cube(int cube_slab[12][9]) {
        // print first block row
        printf("       .------.       \n");
        for (int i = 0; i < 3; i++) {
                printf(" ");
                for (int j = 0; j < 9; j++){
                        if (cube_slab[0][j] != 0) {
                                printf("%s%s%s%s", COLORS[cube_slab[i][j]-1], BLOCK, BLOCK, RESET);
                                if (j == 5) {
                                        printf("|");
                                }
                        } else {
                                printf("  ");
                                if (j == 2) {
                                        printf("|");
                                }
                        }
                }
                printf(" \n");
        }
        printf(".------+------+------.\n");
        for (int i = 3; i < 6; i++) {
                for (int j = 0; j < 9; j++) {
                        if (j == 0 || j == 3 || j == 6) {
                                printf("|");
                        }
                        printf("%s%s%s%s", COLORS[cube_slab[i][j]-1], BLOCK, BLOCK, RESET);
                        
                }
                printf("|\n");
        }
        printf("+------+------+------+\n");
        for (int i = 6; i < 9; i++) {
                printf(" ");
                for (int j = 0; j < 9; j++){
                        if (cube_slab[0][j] != 0) {
                                printf("%s%s%s%s", COLORS[cube_slab[i][j]-1], BLOCK, BLOCK, RESET);
                                if (j == 5) {
                                        printf("|");
                                }
                        } else {
                                printf("  ");
                                if (j == 2) {
                                        printf("|");
                                }
                        }
                }
                printf(" \n");
        }
        printf("       .------.       \n");
        for (int i = 9; i < 12; i++) {
                printf(" ");
                for (int j = 0; j < 9; j++){
                        if (cube_slab[0][j] != 0) {
                                printf("%s%s%s%s", COLORS[cube_slab[i][j]-1], BLOCK, BLOCK, RESET);
                                if (j == 5) {
                                        printf("|");
                                }
                        } else {
                                printf("  ");
                                if (j == 2) {
                                        printf("|");
                                }
                        }
                }
                printf(" \n");
        }
        printf("       +------+       \n");


}


void rotate_cube(int axis, char direction) {
        int dir = 1;
        if (axis <= 3 || (axis >= 7 && axis <= 9)) // When right and left makes sense
        {
                dir = (direction == 'r') ? 1 : 0;
                
        }
        else // When up and down makes sense
        {
                dir = (direction == 'u') ? 1 : 0;
        }

        switch (axis) {
                        case 1:
                                shift_array_3(axis_1, dir);
                                break;
                        case 2:
                                shift_array_3(axis_2, dir);
                                break;
                        case 3:
                                shift_array_3(axis_3, dir);
                                break;
                        case 4:
                                shift_array_3(axis_4, dir);
                                break;
                        case 5:
                                shift_array_3(axis_5, dir);
                                break;
                        case 6:
                                shift_array_3(axis_6, dir);
                                break;
                        case 7:
                                shift_array_3(axis_7, dir);
                                break;
                        case 8:
                                shift_array_3(axis_8, dir);
                                break;
                        case 9:
                                shift_array_3(axis_9, dir);
                                break;
        }
}

void shift_array_3(int *array[], int direction) {
        if (direction) // positive direction: right / cc / up
        {
                int temp[3];
                for (int i = 0; i < 3; i++) {
                        temp[i] = *array[i+9];
                }
                for (int i = 11; i >= 3; i--) {
                        *array[i] = *array[i - 3];
                }
                for (int i = 0; i < 3; i++) {
                        *array[i] = temp[i];;
                }
        } else // negative direction: left / ccw / down
        {
                int temp[3];
                for (int i = 0; i < 3; i++) {
                        temp[i] = *array[i];
                }
                for (int i = 0; i < 9; i++) {
                        *array[i] = *array[i + 3];
                }
                for (int i = 0; i < 3; i++) {
                        *array[i+9] = temp[i];;
                }
        }
}



// Basic functions ---------------------
void move_cursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

void update_char(int row, int col, char c) {
        move_cursor(row, col);
        putchar(c);
        move_cursor(INPUT_ROW, INPUT_COL);
        fflush(stdout);
}


// Settings functions -------------------
void enable_raw_mode() {
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode() {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}