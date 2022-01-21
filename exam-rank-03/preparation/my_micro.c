#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

# define ERR1 "Error: argument\n"
# define ERR2 "Error: Operation file corrupted\n"

int error(char *err)
{
	int i = 0;
	while (err[i] != '\0') // замена strlen
		i++;
	write(1, err, i);
	return (1);
}

int main(int ac, char **av)
{
	FILE *file;

	int wcanv, hcanv;
	char bg;
	char *canvas;
	int i = 0;
	int scan;
	int xa, ya;
	char Rr, color;
	float xtl, ytl, xbr, ybr;

	if (ac != 2)
		return(error(ERR1));
	if (!(file = fopen(av[1], "r")))
		return (error(ERR2));
	else if (fscanf(file, "%d %d %c\n", &wcanv, &hcanv, &bg) != 3)
		return (error(ERR2));
	else if (!(0 < wcanv && wcanv <= 300 && 0 < hcanv && hcanv <= 300)) //проверка размера холста
		return (error(ERR2));
	else if (!(canvas = (char *)malloc(sizeof (char) * (wcanv * hcanv)))) //замаллочили как одну строку - одномерный массив, можно сделать как массив строк двумерный массив
		return (error(ERR2));
	else
		memset(canvas, bg, wcanv * hcanv);
	while ((scan = fscanf(file, "%c %f %f %f %f %c\n", &Rr, &xtl, &ytl, &xbr,
						  &ybr, &color)) == 6)
	{
		if (xbr <= 0 || ybr <= 0 || (Rr != 'r' && Rr != 'R'))
			break;
		xbr = xtl + xbr;
		ybr = ytl + ybr;
		if (xtl - (int)xtl > 0) //округление в большую сторону
			xtl = xtl + 1;
		if (ytl - (int)ytl > 0) //округление в большую сторону
			ytl = ytl + 1;
		ya = 0;
		while (ya != hcanv) //перебор строк
		{
			xa = 0;
			while (xa != wcanv) //перевор столбцов
			{
				if ((int)xtl <= xa && xa <= (int)xbr && (int)ytl <= ya && ya <= (int)ybr) //проверка на то что xa и ya внутри прямогуольника
				{
					if (Rr == 'R')
						canvas[wcanv * ya + xa] = color; //заполняем все пространство прямоугольника
                    if ((int)xtl == xa || (int)ytl == ya || (int)xbr == xa || (int)ybr == ya)
							canvas[wcanv * ya + xa] = color; //заполняем только контур прямоугольника
				}
				xa++;
			}
			ya++;
		}
	}
	if (scan != -1)
	{
		free(canvas);
		return(error(ERR2));
	}
	while (i != hcanv) //вывод холста, то что получилось в итоге
	{
		write(1, canvas + i * wcanv, wcanv); //строками
		write(1, "\n", 1);
		i++;
	}
	free(canvas);
	fclose(file);
	return(0);
}
