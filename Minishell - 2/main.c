#include "parser_pipex.h"

static void	print_command_info(char **args, char *command_path)
{
	int	i;

	printf("valid command: %s\n", args[0]);
	printf("command path: %s\n", command_path);
	if (args[1])
	{
		printf("command arguments:");
		i = 1;
		while (args[i])
		{
			printf(" %s", args[i]);
			i++;
		}
		printf("\n");
	}
}

void	validate_and_print_command(char **args, char *const envp[])
{
	char	*command_path;

	if (!args || !args[0])
		return ;
	command_path = find_command_path(args[0], envp);
	if (command_path)
	{
		print_command_info(args, command_path);
		free(command_path);
	}
	else
		printf("invalid command: %s\n", args[0]);
}

static void	process_command(char **args, int *i, char *const envp[])
{
	char	*command_path;

	command_path = find_command_path(args[*i], envp);
	if (command_path)
	{
		printf("Valid command: %s\n", args[*i]);
		printf("Command path: %s\n", command_path);
		free(command_path);
		if (args[*i + 1] && strcmp(args[*i + 1], "|") != 0)
			printf("Command arguments: %s\n", args[*i + 1]);
	}
	else
		printf("Invalid command: %s\n", args[*i]);
	(*i)++;
	if (args[*i] && strcmp(args[*i], "|") != 0)
		(*i)++;
}

void	parse_and_validate_input(const char *input, char *const envp[])
{
	char	**args2;
	char	**args;
	int		i;
	int		j;

	args2 = parse_command(input);
	args = parse_command_line(args2);
	i = 0;
	while (args[i] != NULL)
	{
		if (strcmp(args[i], "|") == 0)
		{
			printf("Pipe:  |\n");
			i++;
			continue ;
		}
		process_command(args, &i, envp);
	}
	j = 0;
	while (args[j] != NULL)
		free(args[j++]);
	free(args);
}


int main(int argc, char *argv[], char *envp[])
{
    (void)argc;
    (void)argv;
    char *input;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            printf("Exiting minishell...\n");
            break;
        }
        if (*input)
            add_history(input);
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }
        parse_and_validate_input(input, envp);
        free(input);
    }
    return (0);
}

