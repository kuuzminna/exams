#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIDE_OUT	0
#define SIDE_IN		1
// для файловых дескрипторов
#define STDIN		0
#define STDOUT		1
#define STDERR		2
// для поля type
#define TYPE_END	0
#define TYPE_PIPE	1
#define TYPE_BREAK	2

// #ifdef TEST_SH
// # define TEST		1
// #else
// # define TEST		0
// #endif

typedef struct	s_list
{
	char			**args; // команда и параметры
	int				length; // длина массива args
	int				type; // наличие '|' or ';' в иином случае TYPE_END
	int				pipes[2]; // для создания канала
	struct s_list	*previous; // link to the previous node
	struct s_list	*next; // link to the next node
}				t_list;

/*
**====================================
**============Part utils==============
**====================================
*/

int ft_strlen(char const *str) // высчитывает длину строки
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

/*
**====================================
**============Part error==============
**====================================
*/

int show_error(char const *str) // вывод ошибки
{
	if (str)
		write(STDERR, str, ft_strlen(str));
	return (EXIT_FAILURE);
}

int exit_fatal(void) // ошибка выделения памяти, любая др ош
{
	show_error("error: fatal\n");
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

void *exit_fatal_ptr(void) // ошибка и вернуть НААЛЛ. использует в стердапе и все
{
	exit_fatal();
	exit(EXIT_FAILURE);
	return (NULL);
}

char *ft_strdup(char const *str) // Функция strdup() путем обращения к функции malloc() выделяет память,
//  достаточную для хранения дубликата строки, на которую указывает str, а затем производит копирование этой строки
//  в выделенную область и возвращает указатель на нее.
{
	char	*copy;
	int		i;

	if (!(copy = (char*)malloc(sizeof(*copy) * (ft_strlen(str) + 1))))
		return (exit_fatal_ptr());
	i = 0;
	while (str[i])
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = 0;
	return (copy);
}

/*
**====================================
**============Part parsing============
**====================================
*/

int add_arg(t_list *cmd, char *arg)  // указатель на узел и аргумент
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = NULL;
	if (!(tmp = (char**)malloc(sizeof(*tmp) * (cmd->length + 2))))
		return (exit_fatal());
	while (i < cmd->length)
	{
		tmp[i] = cmd->args[i];// записываем команду и ост арг
		i++;
	}
	if (cmd->length > 0) // очистка - если args[i] существует
		free(cmd->args);
	cmd->args = tmp;
	cmd->args[i++] = ft_strdup(arg); // записываем команду или параметр
	cmd->args[i] = 0;
	cmd->length++; //если была команда, то длина = 1, появился аргумент то +1
	return (EXIT_SUCCESS);
}

int list_push(t_list **list, char *arg) // добавление комнады в список двусвязный
{
	t_list	*new; // указатель на узел

	if (!(new = (t_list*)malloc(sizeof(*new)))) // выделение памяти
		return (exit_fatal());
	new->args = NULL;
	new->length = 0;
	new->type = TYPE_END;
	new->previous = NULL;
	new->next = NULL;
	if (*list) // если узел существует
	{
		(*list)->next = new;
		new->previous = *list; // ссылка на предыдущий и следующий узел
	}
	*list = new;
	return (add_arg(new, arg));
}

int list_rewind(t_list **list) // перемотка к первому узлу списка
{
	while (*list && (*list)->previous) // если существует этот узел и предыдущий
		*list = (*list)->previous;		// то переходим к предыдущему
	return (EXIT_SUCCESS);
}

int list_clear(t_list **cmds) // очистка списка
{
	t_list	*tmp; // указатель на узел
	int		i;

	list_rewind(cmds);
	while (*cmds) // пока узел существует
	{
		tmp = (*cmds)->next; // указателю tmp присваеваем адрес следующего узла
		i = 0;
		while (i < (*cmds)->length) // пока i меньше кол-ва команд и аргументов
			free((*cmds)->args[i++]); // очистка каждого аргумента (каждого элемента массива args)
		free((*cmds)->args); // очистка массива args
		free(*cmds); // очистка самого узла
		*cmds = tmp; // переопределение узла списка на следующий
	}
	*cmds = NULL; // зануление узла списка
	return (EXIT_SUCCESS);
}

int parse_arg(t_list **cmds, char *arg)
{
	int	is_break;

	is_break = (strcmp(";", arg) == 0);							//1 - если ";", 0 - если иное
	if (is_break && !*cmds)
		return (EXIT_SUCCESS);
	else if (!is_break && (!*cmds || (*cmds)->type > TYPE_END)) // если не ";" и нет cmds или тип комнады > 0(тип-пайп (1), тип-остановки(2))
		return (list_push(cmds, arg));							// то добавим в лист
	else if (strcmp("|", arg) == 0)								// если пайп
		(*cmds)->type = TYPE_PIPE;								// то тип = тип-пайпа
	else if (is_break) // если ";"
		(*cmds)->type = TYPE_BREAK; 							// то тип = тип-остаговки
	else 														// это аргумент
		return (add_arg(*cmds, arg));
	return (EXIT_SUCCESS);
}

/*
**====================================
**============Part execve=============
**====================================
*/

int exec_cmd(t_list *cmd, char **env)
{
	pid_t	pid;
	int		ret;
	int		status;
	int		pipe_open;

	ret = EXIT_FAILURE;
	pipe_open = 0;
	if (cmd->type == TYPE_PIPE || (cmd->previous && cmd->previous->type == TYPE_PIPE)) // если тип узла пап или предущий тип ущла-пайп
	{
		pipe_open = 1;
		if (pipe(cmd->pipes)) // При успешном выполнении возвращается 0. В случае ошибки возвращается -1
			return (exit_fatal());
	}
	pid = fork(); // создание дочеренго процесса
	// При успешном завершении родителю возвращается PID процесса-потомка, а процессу-потомку возвращается 0.
	//  При ошибке родительскому процессу возвращается -1, процесс-потомок не создаётся
	if (pid < 0)
		return (exit_fatal());
	else if (pid == 0)
	{
		if (cmd->type == TYPE_PIPE
			&& dup2(cmd->pipes[SIDE_IN], STDOUT) < 0)
			return (exit_fatal());
		if (cmd->previous && cmd->previous->type == TYPE_PIPE
			&& dup2(cmd->previous->pipes[SIDE_OUT], STDIN) < 0)
			return (exit_fatal());
		if ((ret = execve(cmd->args[0], cmd->args, env)) < 0)
		{
			show_error("error: cannot execute ");
			show_error(cmd->args[0]);
			show_error("\n");
		}
		exit(ret);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (pipe_open)
		{
			close(cmd->pipes[SIDE_IN]);
			if (!cmd->next || cmd->type == TYPE_BREAK)
				close(cmd->pipes[SIDE_OUT]);
		}
		if (cmd->previous && cmd->previous->type == TYPE_PIPE)
			close(cmd->previous->pipes[SIDE_OUT]);
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
	}
	return (ret);
}

int exec_cmds(t_list **cmds, char **env)
{
	t_list	*crt;
	int		ret;

	ret = EXIT_SUCCESS;
	list_rewind(cmds);
	while (*cmds)
	{
		crt = *cmds;
		if (strcmp("cd", crt->args[0]) == 0)					// if args = cd
		{
			ret = EXIT_SUCCESS;
			if (crt->length < 2) 								// если cd имеет неправильный номер аргумента
				ret = show_error("error: cd: bad arguments\n");
			else if (chdir(crt->args[1]))						// путь для перехода. если cd не удался
			{
				ret = show_error("error: cd: cannot change directory to ");
				show_error(crt->args[1]);
				show_error("\n");
			}
		}
		else
			ret = exec_cmd(crt, env);
		if (!(*cmds)->next)										// если следующей комнады нет, то прекратить
			break ;
		*cmds = (*cmds)->next;
	}
	return (ret);
}

int main(int argc, char **argv, char **env)
{
	t_list	*cmds;
	int		i;
	int		ret;

	ret = EXIT_SUCCESS;
	cmds = NULL;
	i = 1;
	while (i < argc)
		parse_arg(&cmds, argv[i++]);
	if (cmds)
		ret = exec_cmds(&cmds, env);
	list_clear(&cmds);
	// if (TEST)
	// 	while (1);
	return (ret);
}