#include "parser_pipex.h"

void	free_command_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static char	*get_path_from_env(char *const envp[])
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*check_direct_executable(char *command)
{
	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	return (NULL);
}

static char	*search_in_path(char *command, char **path_dirs)
{
	int		i;
	char	*temp;
	char	*full_path;

	i = 0;
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i++], "/");
		if (!temp)
		{
			ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO);
			return (NULL);
		}
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (!full_path)
		{
			ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

char	*find_command_path(char *command, char *const envp[])
{
	char	*path;
	char	**path_dirs;
	char	*result;

	result = check_direct_executable(command);
	if (result)
		return (result);
	path = get_path_from_env(envp);
	if (!path)
		return (NULL);
	path_dirs = ft_split(path, ':');
	if (!path_dirs)
	{
		ft_putstr_fd("Error: ft_split failed\n", STDERR_FILENO);
		exit(1);
	}
	result = search_in_path(command, path_dirs);
	free_command_args(path_dirs);
	if (result)
		return (result);
	else
		return (check_direct_executable(command));
}
