#include "get_next_line.h"

int	get_next_line(char **line)
{
	int		rd;
	char	ch;
	int		i;

	char	*buf = malloc(100000);
	*line = buf;
	i = 0;
	while ((rd= read(0, &ch, 1) > 0) && ch!= '\n')
	{
		buf[i++] = ch;
	}
	buf[i] = '\0';
	return (rd);
}
