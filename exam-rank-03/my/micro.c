#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int height, width;
char type, color, back;
char *draw;
float x, y, w, h;

int check_hit(float i, float j)
{
    if (i < x || i > x + w || j < y || j > y + h )//запомнить
        return (0);
    if (i - x < 1 || x + w - i < 1 || j - y < 1 || y + h - j < 1)//запомнить
        return (2);
    return(1);
}

void drawing(void)
{
    int i = -1, j, hit;
    while (++i < width)
    {
        j = -1;
        while (++j < height)
        {
            hit = check_hit((float)i, (float)j);
            if ((hit == 1 && type == 'R') || hit == 2) //забываю что 1, а что 2
                draw[i + j * height] = color;//обрати внимание
        }
    }
}

int parser(FILE *file)
{
    int i = 0, result;
    if (scanf(file, "%d %d %c\n", &width, &height, &back) != 3)
        return (1);
    if (width <= 0 || width > 300 || height <= 0 || height > 300)
        return (1);
    draw = malloc(width * height);
    if (draw == NULL)
        return (1);
    while(i < width * height) //забыла
        draw[i++] = back; //забыла
    while ((result = scanf(file, "%c %f %f %f %f %c\n", &type, &x, &y, &w, &h, &color)) == 6)
    {
        if ((type != 'r' && type != 'R') || w <=  0|| h <= 0)
            return (1);
        drawing();
    }
    if (result != -1)
        return(1);
    return(0);
}

void    output(void)
{
    int i = -1, c = 10;

    while (++i < height)
    {
        write(1, draw + i * width, width);
        write(1, &c, 1);
    }
}

int main(int ac, char **av)
{
    FILE *file;

    if (ac != 2)
    {
        write(1, "Error: argument\n", 16);
        return (1);
    }
    if(!(file = fopen(av[1], "r" || parser(file))))
    {
        write(1, "Error: Operation file corrupted\n", 32);
        return (1);
    }
    output();
    fclose(file);
    free(draw);
    return (0);
}