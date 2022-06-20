#include <stdio.h>
#include <ncurses.h>
#include <functions.h>
#include <tetrimino.h>

int main()
{
    int a[2][3] = {
        {1, 1, 1},
        {0, 0, 1}
    };
    int b[1][4] = {
        {1, 1, 1, 1}
    };
    int c[3][3] = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    };
    int safe1 = blank_cells(&a[0][0], 2, 3);
    int safe2 = blank_cells(&b[0][0], 1, 4);
    int safe3 = blank_cells(&c[0][0], 3, 3);
    printf("%d %d %d\n", safe1, safe2, safe3);
}