#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int     width, height;
char    *draw;
char    type, color, back;
float   x, y, rad;

int check_hit()
{

}

void drawing(void)
{

}

int parser(FILE *file)
{

}

void output(void)
{
    int i = -1, c = 10;
    while (++i < height)
    {
        write(1, draw + i * width, )
    }

}

int main(int ac, char **av)
{
    FILE    *file;

    if (ac != 2)
    {
        write(1, "Error: argument\n", 16);
        return (1);
    }
    if (!(file = fopen(av[1], "r") || parser(file)))
    {
        write(1, "Error: Operation file corrupted\n", 32);
        return (1);
    }
    output();
    fclose(file);
    free(draw);
    return (0);
}
