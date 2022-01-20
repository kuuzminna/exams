#include "get_next_line.h"

char *ft_strjoin(char *s1, char *s2)
{
    int i;
    char *res;

    i = 0;
    while (s1[i])
        i++;
    res = (char *)malloc(sizeof(char) * (i + 2));
    i = 0;
    while(s1[i])
    {
        res[i] = s1[i];
        i++;
    }
        res[i++] = s2[0];
    res[i] = '\0';
    free(s1);
    return(res);
}

int get_next_line(char **line)
{
    char buf[2];
    int i;

    i = 0;
    if (!line || read(0, NULL, 0) < 0)
        return (-1);
    *line = (char *)malloc(1);
    **line = '\0';
    while (read(0, buf, 1) > 0)
    {
        buf[1] = '\0';
        if (buf[0] == '\n')
            return (1);
        *line = ft_strjoin(*line, buf);
    }
    return (0);
}
