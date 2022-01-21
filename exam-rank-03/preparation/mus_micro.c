#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	int	canv_w, canv_h; //ширина и высота холста (поля где рисуе)
	int	canv_len = 0;
	char	bg, color, Rr; //bg - символ для фона, color - чем рисуем, Rr - тип прямоугольника
	int	res, y, x1, y1;
	char	*canvas; //массив холста не более 300х300
	float	xtl, ytl, width, height, xbr, ybr;

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
		
	//читаем команды и рисуем прямоугольники
	while ((res = fscanf(file, "%c %f %f %f %f %c\n", &Rr, &xtl, &ytl, &width, &height, &color)) == 6)
	{
		//проверка валидности параметров
		if (width <= 0 || height <=0 || (Rr != 'R' && Rr != 'r'))
			break;

		xbr = xtl + width;
		ybr = ytl + height;

		//округляем в большую для положительных чисел
		if (xtl - (int)xtl > 0)
			xtl = xtl + 1;
		if (ytl - (int)ytl > 0)
			ytl = ytl + 1;
		//перебираем ячейки холста
		y = 0;
		while (y < canv_len)
		{
			y1 = y / canv_w;
			x1 = y % canv_w;
			// printf("%d %d %d\n", y, y1, x1);

			if (x1 >= (int)xtl && y1 >= (int)ytl && x1 <= (int)xbr && y1 <= (int)ybr) //если входит в прям-к
			{
				if (Rr == 'R')
					canvas[y] = color;
				if (x1 == (int)xtl || y1 == (int)ytl || x1 == (int)xbr || y1 == (int)ybr) //если попадает в контур
					canvas[y] = color;
			}
			y++;
		}
	}

	// printf("%d %d\n", res, canv_len);
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
