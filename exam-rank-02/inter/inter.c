#include <unistd.h>

void inter(char *str1, char *str2)
{
	int k;
	int count;
	int i;
	int j;

	i = 0;
	j = 0;
	while(str1[i])
	{
		count = 0;
		k = 0;
		while (k <= i)
		{
			if (str1[i] == str1[k])
				count++;
			k++;
		}

		if (count == 1)
		{
			j = 0;
			while (str2[j])
			{
				if (str1[i] == str2[j])
				{
					write(1, &str1[i], 1);
					break;
				}
				j++;
			}
		}
		i++;
	}
}

int main(int argc, char **argv)
{
	if (argc == 3)
		inter(argv[1], argv[2]);
	write(1, "\n", 1);
	return (0);
}