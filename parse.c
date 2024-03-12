/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:36:14 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/12 15:59:27 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char **envp)
{
	char	**strs;
	char	*tmp;
	int		i;

	strs = NULL;
	i = -1;
	while (envp[++i])
		if (!ft_strncmp(envp[i], "PATH=", 5))
			strs = ft_split(envp[i], ':');
	tmp = malloc(ft_strlen(strs[0]) - 4);
	i = 4;
	while (strs[0][++i])
		tmp[i - 5] = strs[0][i];
	tmp[i - 5] = '\0';
	free(strs[0]);
	strs[0] = malloc(ft_strlen(tmp) + 1);
	i = -1;
	while (tmp[++i])
		strs[0][i] = tmp[i];
	strs[0][i] = '\0';
	free(tmp);
	return (strs);
}

char	*join_path(char *path, char *file)
{
	char	*str;
	char	*str2;

	str = ft_strjoin(path, "/", 0);
	str2 = ft_strjoin(str, file, 1);
	return (str2);
}

char	*try_path(char **strs, char *str)
{
	int		i;
	char	*path;

	i = 0;
	while (strs[i])
	{
		path = join_path(strs[i], str);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	free(str);
	return (NULL);
}

int	ft_execve(char *path, char **argv, char **envp)
{
	pid_t	p;
	
	p = fork();
	if (p < 0)
		return (1);
	if (p == 0)
		execve(path, argv, envp);
	waitpid(p, NULL, 0);
	return (0);
}

void	exe_command(char *command, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*cmd_err;

	cmd = ft_split(command, ' ');
	if (!exe_builtin(cmd, envp))
		return ;
	cmd_err = ft_strdup(cmd[0]);
	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
		ft_execve(cmd[0], cmd, envp);
	else
	{
		paths = get_paths(envp);
		cmd[0] = try_path(paths, cmd[0]);
		tab_free(paths);
		if (!cmd[0])
		{
			ft_putstr_fd("command not found: ", 2);
			ft_putstr_fd(cmd_err, 2);
			ft_putstr_fd("\n", 2);
		}
		else
			ft_execve(cmd[0], cmd, envp);
	}
}

int	is_valid_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (0);
	return (1);
}

void	parse_buffer(char *buffer, char **envp, t_instruct instruct)
{
	int		i;
	int		start;
	char	*tmp;

	i = -1;
	start = 0;
	(void)instruct;
	while (buffer[++i])
	{
		if (!is_valid_char(buffer[i]))
		{
			tmp = ft_substr(buffer, start, i - start);
			start = i + 1;
			while (!is_valid_char(buffer[++i]) || buffer[i] == ' ')
				start++;
			exe_command(tmp, envp);
			free(tmp);
		}
	}
	if (start != i)
	{
		tmp = ft_substr(buffer, start, i);
		exe_command(tmp, envp);
		free(tmp);
	}
}
