#include <unistd.h>

void	ft_union(char *str1, char *str2)
{
	int k;
	int count;
	int i;
	int j;

	i = 0;
	while(str1[i])
	{
		k = 0;
		count = 0;
		while (k <= i)
		{
			if (str1[i] == str1[k])
				count++;
			k++;
		}
		if (count == 1)
			write(1, &str1[i], 1);
		i++;
	}
	j = 0;
	while (str2[j])
	{
		i = 0;
		count = 0;
		while (str1[i])
		{
			if (str2[j] == str1[i])
				count++;
			i++;
		}
		k = 0;
		while (k <= j)
		{
			if (str2[j] == str2[k])
				count++;
			k++;
		}
		if (count == 1)
			write(1, &str2[j], 1);
		j++;
	}
}

int main(int argc, char **argv)
{
	if (argc == 3)
		ft_union(argv[1], argv[2]);
	write(1, "\n", 1);
	return (0);
}