#include "microshell.h"

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

void ft_putstr_fd(char *str, int fd)
{
    write(fd, str, ft_strlen(str));
}

void ft_exec_fatal(char *str)
{
    ft_putstr_fd("error: cannot execute ", 2);
    ft_putstr_fd(str, 2);
    ft_putstr_fd("\n", 2);
    exit(1);
}

void ft_fatal(void)
{
    ft_putstr_fd("error: fatal\n", 2);
    exit(1);
}

void	ft_openpipes(int fd[2])
{
	if (has_pipe == 1)
	{
		if (close(fd[READ]) == -1)
			ft_fatal();
		if (dup2(fd[WRITE], STDOUT_FILENO) == -1)
			ft_fatal();
		if (close(fd[WRITE]) == -1)
			ft_fatal();
	}
}

void	ft_closepipes(int fd[2])
{
	if (has_pipe == 1)
	{
		if (dup2(fd[READ], STDIN_FILENO) == -1)
			ft_fatal();
		if (close(fd[READ]) == -1)
			ft_fatal();
		if (close(fd[WRITE]) == -1)
			ft_fatal();
	}
}

void ft_cd(char **argv)
{
    int i = 0;

    while (argv[i] != NULL)
        i++;
    if (i != 2)
    {
        ft_putstr_fd("error: cd: bad arguments\n", 2);
        ret = 1;
        return ;
    }
    if (chdir(argv[1]) == -1)
    {
        ft_putstr_fd("error: cd: cannot change directory to ", 2);
        ft_putstr_fd(argv[1], 2);
        ret = 1;
        return ;
    }
}

void ft_execute(char **argv, char **envp)
{
    pid_t pid;
    int fd[2];

    if (strcmp(argv[0], "cd") == 0)
        return (ft_cd(argv));
    if (has_pipe == 1)
    {
        if (pipe(fd) == -1)
            ft_fatal();
    }
    pid = fork();
    if (pid == 0)
    {
        ft_openpipes(fd);
        if (execve(argv[0], argv, envp) == -1)
            ft_exec_fatal(argv[0]);
    }
    else
        ft_closepipes(fd);
}

void ft_command(char **cmd, char **envp)
{
    int i = -1;
    int begin = 0;
    int nproc = 0;

    while (cmd[++i] != NULL)
    {
        if (strcmp(cmd[i], "|") == 0 || cmd[i + 1] == NULL)
        {
            has_pipe = 0;
            if (strcmp(cmd[i] , "|") == 0)
            {
                has_pipe = 1;
                cmd[i] = NULL;
            }
            ft_execute(cmd + begin, envp);
            begin = i + 1;
            nproc++;
        }
    }
    while (nproc-- > 0)
        waitpid(-1, 0, 0);
}

void	ft_restorefd(void)
{
	int		tmp;

	tmp = dup(STDIN_FILENO);
	if (dup2(to_stdin, STDIN_FILENO) == -1)
		ft_fatal();
	if (close(tmp) == -1)
		ft_fatal();
}

int main(int argc, char **argv, char **envp)
{
    int i = 0;
    int begin = 1;

    to_stdin = dup(STDIN_FILENO);
    (void)argc;
    while(argv[++i] != NULL)
    {
        if (strcmp(argv[i], ";") == 0 || argv[i + 1] == NULL)
        {
            if (strcmp(argv[i], ";") == 0)
                argv[i] = NULL;
            ft_command(argv + begin, envp);
            begin = i + 1;
        }
        ret = 0;
        ft_restorefd();
    }
    return (ret);
}