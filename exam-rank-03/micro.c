#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int width, height;
char background, color, type;
char *draw;

float x, y, w, h;


int check_hit(float i, float j)
{

}
void drawing(void)
{
    int i=-1, j;
    int hit;
    while(++i < width)
    {
        j =-1;
        while (++j < height)
        {
            hit = check_hit((float)i, (float)j);
            if((hit == 1 && type =='R') || hit == 2)
                draw[i * height + j] = color;
        }
        
    }
}
int parser(FILE *file)
{
    int i = 0, result;

    if (fscanf(file, "%d %d %c\n", width, height, background) != 3)
        return(1);
    if (width < 1 || width > 300 || height < 1 || height > 300)
        return(1);
    draw = malloc(width * height);
    if (draw == NULL)
        return(1);
    while (i < width * height)
        draw[i++] = background;
    while ((result = fscanf(file, "%c %d %d %d %d %c", &type, &x, &y, &w, &h, &color)) == 6)
    {
        if ((type != 'r' && type != 'R') || w <= 0 || h <= 0)
            return(1);
        drawing();
    }
    if (result != -1)
        return(1);
    return(0);
}

void output (void)
{
    int i = -1, c = 10;
    while (++i < height)
    {
        write(1,draw + i * width, width);//запомнить второй параметр
        write(1, &c, 1);
    }
    
}

int main(int ac, char **av)
{
    FILE *file;

    if (ac != 2)
    {
        write (1, "Error: argument\n", 16);
        return(1);
    }
    if (!(file = fopen(av[1], "r") || parser(file)))
    {
        write(1, "Error: Operation file corrupted\n", 32);
        return(1);
    }
    output();
    fclose(file);
    free(draw);
    return(0);
}