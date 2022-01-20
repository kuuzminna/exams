#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int	ft_error(char *s)
{
	int i = 0;

	while (s[i] != '\0')
		i++;
	write(1, s, i);
	write(1, "\n", 1);
	return (1);
}

int	main(int argc, char **argv)
{
	FILE	*file;
	int		canv_w, canv_h; //ширина и высота холста (поля где рисуе)
	int		canv_len = 0;
	char	bg, color, Cc; //bg - символ для фона, color - чем рисуем, Rr - тип прямоугольника
	int		res, y, x1, y1;
	char	*canvas; //массив холста не более 300х300
	float	xc, yc, dx, dy, d, r, dr;

	if (argc != 2)
		return(ft_error("Error: argumen"));
	if (!(file = fopen(argv[1], "r")))
		return(ft_error("Error: Operation file corrupted"));
	//parser
	res = fscanf(file, "%d %d %c\n", &canv_w, &canv_h, &bg); //читаем 1ю строчку
	if (res != 3)//сработает если аргументов меньше или они не соответствуют формату
		return(ft_error("Error: Operation file corrupted"));
	else if (canv_w < 1 || canv_w > 300 || canv_h < 1 || canv_h > 300) //проверка размера холста
		return(ft_error("Error: Operation file corrupted"));
	else if (!(canvas = malloc(canv_len = canv_w * canv_h)))
		return(ft_error("Error: Operation file corrupted"));
	else
		memset(canvas, bg, canv_len); //заполняем холст символом bg
	//читаем команды и рисуем курги
	while ((res = fscanf(file, "%c %f %f %f %c\n", &Cc, &xc, &yc, &r, &color)) == 5)
	{
		//проверка валидности параметров
		if (r <= 0 || (Cc != 'C' && Cc != 'c'))
			break;
		//перебираем ячейки холста
		y = 0;
		while (y < canv_len)
		{
			y1 = y / canv_w;
			x1 = y % canv_w;
			dx = xc - (float)x1;
			dy = yc - (float)y1;
			d = sqrtf(dx * dx + dy * dy);
			dr = r - d;

			if (d <= r) //если входит в круг
			{
				if (Cc == 'C')
					canvas[y] = color;
				if (dr < 1 && Cc == 'c') //если попадает в контур
					canvas[y] = color;
			}
			y++;
		}
	}

	if (res != -1)
	{
		fclose (file);
		free(canvas);
		return (ft_error("Error: Operation file corrupted"));
	}

	//вывод холста строками
	y = 0;
	while (y < canv_h)
	{
		write(1, &canvas[y * canv_w], canv_w);
		write(1, "\n", 1);
		y++;
	}
	fclose (file);
	free(canvas);
	return (0);
}
