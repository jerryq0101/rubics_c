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
void highlight_axis_num(int axis);
void refresh_cube();
void increment_axis(int up);
void print_info();

#define EXIT_CHAR '~'
#define INPUT_ROW 19
#define INPUT_COL 1

#define BLOCK "\u2588"
#define BLOCK_HALF "\u2593"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

const char *COLORS[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
/*
6 sides of a cube
*/
int cube_slab[12][9] = {{0}};
int cube_selected[12][9] = {{0}};

/*
9 rotation axis
*/
int *axis_1[12] = {*(cube_slab + 2) + 3, *(cube_slab + 2) + 4, *(cube_slab + 2) + 5, *(cube_slab + 3) + 6, *(cube_slab + 4) + 6, *(cube_slab + 5) + 6, *(cube_slab + 6) + 5, *(cube_slab + 6) + 4, *(cube_slab + 6) + 3, *(cube_slab + 5) + 2, *(cube_slab + 4) + 2, *(cube_slab + 3) + 2};
int *axis_2[12] = {*(cube_slab + 1) + 3, *(cube_slab + 1) + 4, *(cube_slab + 1) + 5, *(cube_slab + 3) + 7, *(cube_slab + 4) + 7, *(cube_slab + 5) + 7, *(cube_slab + 7) + 5, *(cube_slab + 7) + 4, *(cube_slab + 7) + 3, *(cube_slab + 5) + 1, *(cube_slab + 4) + 1, *(cube_slab + 3) + 1};
int *axis_3[12] = {*(cube_slab + 0) + 3, *(cube_slab + 0) + 4, *(cube_slab + 0) + 5, *(cube_slab + 3) + 8, *(cube_slab + 4) + 8, *(cube_slab + 5) + 8, *(cube_slab + 8) + 5, *(cube_slab + 8) + 4, *(cube_slab + 8) + 3, *(cube_slab + 5) + 0, *(cube_slab + 4) + 0, *(cube_slab + 3) + 0};
int *axis_4[12] = {*(cube_slab) + 3, *(cube_slab + 1) + 3, *(cube_slab + 2) + 3, *(cube_slab + 3) + 3, *(cube_slab + 4) + 3, *(cube_slab + 5) + 3, *(cube_slab + 6) + 3, *(cube_slab + 7) + 3, *(cube_slab + 8) + 3, *(cube_slab + 9) + 3, *(cube_slab + 10) + 3, *(cube_slab + 11) + 3};
int *axis_5[12] = {*(cube_slab) + 4, *(cube_slab + 1) + 4, *(cube_slab + 2) + 4, *(cube_slab + 3) + 4, *(cube_slab + 4) + 4, *(cube_slab + 5) + 4, *(cube_slab + 6) + 4, *(cube_slab + 7) + 4, *(cube_slab + 8) + 4, *(cube_slab + 9) + 4, *(cube_slab + 10) + 4, *(cube_slab + 11) + 4};
int *axis_6[12] = {*(cube_slab) + 5, *(cube_slab + 1) + 5, *(cube_slab + 2) + 5, *(cube_slab + 3) + 5, *(cube_slab + 4) + 5, *(cube_slab + 5) + 5, *(cube_slab + 6) + 5, *(cube_slab + 7) + 5, *(cube_slab + 8) + 5, *(cube_slab + 9) + 5, *(cube_slab + 10) + 5, *(cube_slab + 11) + 5};
int *axis_7[12] = {*(cube_slab + 3) + 0, *(cube_slab + 3) + 1, *(cube_slab + 3) + 2, *(cube_slab + 3) + 3, *(cube_slab + 3) + 4, *(cube_slab + 3) + 5, *(cube_slab + 3) + 6, *(cube_slab + 3) + 7, *(cube_slab + 3) + 8, *(cube_slab + 9) + 5, *(cube_slab + 9) + 4, *(cube_slab + 9) + 3};
int *axis_8[12] = {*(cube_slab + 4) + 0, *(cube_slab + 4) + 1, *(cube_slab + 4) + 2, *(cube_slab + 4) + 3, *(cube_slab + 4) + 4, *(cube_slab + 4) + 5, *(cube_slab + 4) + 6, *(cube_slab + 4) + 7, *(cube_slab + 4) + 8, *(cube_slab + 10) + 5, *(cube_slab + 10) + 4, *(cube_slab + 10) + 3};
int *axis_9[12] = {*(cube_slab + 5) + 0, *(cube_slab + 5) + 1, *(cube_slab + 5) + 2, *(cube_slab + 5) + 3, *(cube_slab + 5) + 4, *(cube_slab + 5) + 5, *(cube_slab + 5) + 6, *(cube_slab + 5) + 7, *(cube_slab + 5) + 8, *(cube_slab + 11) + 5, *(cube_slab + 11) + 4, *(cube_slab + 11) + 3};


int axis = 0;

/* For Terminal Settings in RAW mode */
struct termios orig_termios;

int main(void)
{
        enable_raw_mode();

        setup_cube(cube_slab);
        highlight_axis_num(axis);
        refresh_cube();

        print_info();

        // Ready for input
        move_cursor(INPUT_ROW, INPUT_COL);

        int c;
        while ((c = getchar()) != EXIT_CHAR)
        {
                switch (c)
                {
                case 'w':
                        increment_axis(1);
                        break;
                case 's':
                        increment_axis(0);
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
                refresh_cube();
                print_info();

                // Input space
                move_cursor(INPUT_ROW, INPUT_COL);
        }

        disable_raw_mode();
        return 0;
}

// Information Box ---------------------
void print_info()
{
        move_cursor(3, 60);
        printf("Rubik's Cube Slab:\n");
        move_cursor(4, 60);
        printf("Current Axis: %d\n", axis);
        move_cursor(5, 60);
        printf("Instructions:\n");
        move_cursor(6, 60);
        printf("- Use 'w' and 's' to cycle through the axes\n");
        move_cursor(7, 60);
        printf("- Use 'r' for right/down and 'l' for left/up cube rotations \n");
        move_cursor(8, 60);
        printf("- Press '~' to exit the program\n");
        move_cursor(10, 60);
        printf("@jerryq0101");
}

// Cube Functions ----------------------
void refresh_cube()
{
        printf("\033[2J\033[H");
        print_cube(cube_slab);
}

void increment_axis(int up)
{
        highlight_axis_num(axis);
        if (up)
        {
                axis = axis == 10 ? 0 : axis + 1;
        }
        else
        {
                axis = axis < 0 ? 9 : axis - 1;
        }
        highlight_axis_num(axis);
}

// Array processing --------------------
void shift_array_3(int *array[], int direction);
void print_block(int start_row, int start_col);
void toggle_highlight(int **selected_axis);

// Refactor this to pointer operations afterwards to flex
void setup_cube(int cube_slab[12][9])
{
        // load first square
        for (int j = 3; j < 6; j++)
        {
                for (int i = 0; i < 3; i++)
                {
                        cube_slab[i][j] = 1;
                }
        }
        // second square
        for (int i = 3; i < 6; i++)
        {
                for (int j = 0; j < 3; j++)
                {
                        cube_slab[i][j] = 2;
                }
        }

        // third square
        for (int i = 3; i < 6; i++)
        {
                for (int j = 3; j < 6; j++)
                {
                        cube_slab[i][j] = 3;
                }
        }

        // fourth square
        for (int i = 3; i < 6; i++)
        {
                for (int j = 6; j < 9; j++)
                {
                        cube_slab[i][j] = 4;
                }
        }

        // fifth square
        for (int i = 6; i < 9; i++)
        {
                for (int j = 3; j < 6; j++)
                {
                        cube_slab[i][j] = 5;
                }
        }

        // sixth square
        for (int i = 9; i < 12; i++)
        {
                for (int j = 3; j < 6; j++)
                {
                        cube_slab[i][j] = 6;
                }
        }
}

void print_cube(int cube_slab[12][9])
{
        // print first block row
        printf("       .------.       \n");
        for (int i = 0; i < 3; i++)
        {
                printf(" ");
                for (int j = 0; j < 9; j++)
                {
                        if (cube_slab[0][j] != 0)
                        {
                                int color_value = cube_slab[i][j];
                                int is_highlighted = (color_value & (1 << 7)) != 0; // Check if the 8th bit is set
                                if (is_highlighted)
                                {
                                        color_value &= ~(1 << 7); // Clear the 8th bit to get the original color value
                                        printf("%s%s%s%s", COLORS[color_value - 1], BLOCK_HALF, BLOCK_HALF, RESET);
                                }
                                else
                                {
                                        printf("%s%s%s%s", COLORS[color_value - 1], BLOCK, BLOCK, RESET);
                                }

                                if (j == 5)
                                {
                                        printf("|");
                                }
                        }
                        else
                        {
                                printf("  ");
                                if (j == 2)
                                {
                                        printf("|");
                                }
                        }
                }
                printf(" \n");
        }
        printf(".------+------+------.\n");
        for (int i = 3; i < 6; i++)
        {
                for (int j = 0; j < 9; j++)
                {
                        if (j == 0 || j == 3 || j == 6)
                        {
                                printf("|");
                        }
                        int color_value = cube_slab[i][j];
                        int is_highlighted = (color_value & (1 << 7)) != 0; // Check if the 8th bit is set
                        if (is_highlighted)
                        {
                                color_value &= ~(1 << 7); // Clear the 8th bit to get the original color value
                                printf("%s%s%s%s", COLORS[color_value - 1], BLOCK_HALF, BLOCK_HALF, RESET);
                        }
                        else
                        {
                                printf("%s%s%s%s", COLORS[color_value - 1], BLOCK, BLOCK, RESET);
                        }
                }
                printf("|\n");
        }
        printf("+------+------+------+\n");
        for (int i = 6; i < 9; i++)
        {
                printf(" ");
                for (int j = 0; j < 9; j++)
                {
                        if (cube_slab[0][j] != 0)
                        {
                                int color_value = cube_slab[i][j];
                                int is_highlighted = (color_value & (1 << 7)) != 0; // Check if the 8th bit is set
                                if (is_highlighted)
                                {
                                        color_value &= ~(1 << 7); // Clear the 8th bit to get the original color value
                                        printf("%s%s%s%s", COLORS[color_value - 1], BLOCK_HALF, BLOCK_HALF, RESET);
                                }
                                else
                                {
                                        printf("%s%s%s%s", COLORS[color_value - 1], BLOCK, BLOCK, RESET);
                                }
                                if (j == 5)
                                {
                                        printf("|");
                                }
                        }
                        else
                        {
                                printf("  ");
                                if (j == 2)
                                {
                                        printf("|");
                                }
                        }
                }
                printf(" \n");
        }
        printf("       .------.       \n");
        for (int i = 9; i < 12; i++)
        {
                printf(" ");
                for (int j = 0; j < 9; j++)
                {
                        if (cube_slab[0][j] != 0)
                        {
                                int color_value = cube_slab[i][j];
                                int is_highlighted = (color_value & (1 << 7)) != 0; // Check if the 8th bit is set
                                if (is_highlighted)
                                {
                                        color_value &= ~(1 << 7); // Clear the 8th bit to get the original color value
                                        printf("%s%s%s%s", COLORS[color_value - 1], BLOCK_HALF, BLOCK_HALF, RESET);
                                }
                                else
                                {
                                        printf("%s%s%s%s", COLORS[color_value - 1], BLOCK, BLOCK, RESET);
                                }
                                if (j == 5)
                                {
                                        printf("|");
                                }
                        }
                        else
                        {
                                printf("  ");
                                if (j == 2)
                                {
                                        printf("|");
                                }
                        }
                }
                printf(" \n");
        }
        printf("       +------+       \n");
}

void rotate_cube(int axis, char direction)
{
        int dir = 1;
        if (direction == 'r' || direction == 'l')
        {
                dir = (direction == 'r') ? 1 : 0;
        }
        else
        {
                return;
        }

        switch (axis)
        {
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

void shift_array_3(int *array[], int direction)
{
        if (direction) // positive direction: right / cc / up
        {
                int temp[3];
                for (int i = 0; i < 3; i++)
                {
                        temp[i] = *array[i + 9];
                }
                for (int i = 11; i >= 3; i--)
                {
                        *array[i] = *array[i - 3];
                }
                for (int i = 0; i < 3; i++)
                {
                        *array[i] = temp[i];
                        ;
                }
        }
        else // negative direction: left / ccw / down
        {
                int temp[3];
                for (int i = 0; i < 3; i++)
                {
                        temp[i] = *array[i];
                }
                for (int i = 0; i < 9; i++)
                {
                        *array[i] = *array[i + 3];
                }
                for (int i = 0; i < 3; i++)
                {
                        *array[i + 9] = temp[i];
                        ;
                }
        }
}

void toggle_highlight(int **selected_axis)
{
        for (int i = 0; i < 12; i++)
        {
                if ((*(selected_axis[i]) & (1 << 7)) != 0) // Highlight off
                {
                        *(selected_axis[i]) &= ~(1 << 7); // Set the 8th bit to 0
                }
                else // Highlight on
                {
                        *(selected_axis[i]) |= (1 << 7); // Set the 8th bit (MSB) to indicate highlighting
                }
        }
}

void highlight_axis_num(int axis)
{
        switch (axis)
        {
        case 1:
                toggle_highlight(axis_1);
                break;
        case 2:
                toggle_highlight(axis_2);
                break;
        case 3:
                toggle_highlight(axis_3);
                break;
        case 4:
                toggle_highlight(axis_4);
                break;
        case 5:
                toggle_highlight(axis_5);
                break;
        case 6:
                toggle_highlight(axis_6);
                break;
        case 7:
                toggle_highlight(axis_7);
                break;
        case 8:
                toggle_highlight(axis_8);
                break;
        case 9:
                toggle_highlight(axis_9);
                break;
        }
        return;
}

// Basic functions ---------------------
void move_cursor(int row, int col)
{
        printf("\033[%d;%dH", row, col);
}

void update_char(int row, int col, char c)
{
        move_cursor(row, col);
        putchar(c);
        move_cursor(INPUT_ROW, INPUT_COL);
        fflush(stdout);
}

// Settings functions -------------------
void enable_raw_mode()
{
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode()
{
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}