/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:36:14 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/13 16:55:08 by tgriblin         ###   ########.fr       */
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

void	close_all_pipes(t_instruct *ins, int close_before, int close_curr)
{
	int	i;

	i = -1;
	while (++i < ins->size)
	{
		if (i == ins->ind  && !close_curr)
			continue ;
		else if (i == ins->ind - 1 && !close_before)
			continue ;
		else
		{
			close(ins->pipes[i][0]);
			close(ins->pipes[i][1]);
		}
	}
}

void	dup_fds(t_instruct *ins, int do_pipe)
{
	if (do_pipe == 0)
		close_all_pipes(ins, 1, 0);
	if (do_pipe == 1)
	{
		dup2(ins->pipes[ins->ind - 1][0], 0);
		close(ins->pipes[ins->ind - 1][1]);
		close_all_pipes(ins, 0, 1);
	}
	if (do_pipe == 2)
	{
		dup2(ins->pipes[ins->ind][1], 1);
		close(ins->pipes[ins->ind][0]);
		close_all_pipes(ins, 1, 0);
	}
	if (do_pipe == 3)
	{
		dup2(ins->pipes[ins->ind - 1][0], 0);
		close(ins->pipes[ins->ind - 1][1]);
		dup2(ins->pipes[ins->ind][1], 1);
		close(ins->pipes[ins->ind][0]);
		close_all_pipes(ins, 0, 0);
	}
}

int	ft_execve(char *path, char **argv, t_instruct *ins, int do_pipe)
{
	pid_t	p;
	char	**envp;

	if (ins)
		envp = ins->envp;
	else
		envp = NULL;
	p = fork();
	if (p < 0)
		return (ft_putstr_fd("fork error", 2), 1);
	if (p == 0)
	{
		dup_fds(ins, do_pipe);
		execve(path, argv, envp);
	}
	waitpid(p, NULL, 0);
	return (0);
}

void	call_ft_execve(char **cmd, t_instruct *ins)
{
	int	ind;

	if (ins)
		ind = ins->ind;
	if (ins && ind <= ins->size)
	{
		if (!ind && !ins->size)
			ft_execve(cmd[0], cmd, ins, 0);
		if (!ind && ins->i_tab[ind] == PIPE)
			ft_execve(cmd[0], cmd, ins, 2);
		if (ind && ins->i_tab[ind - 1] == PIPE && ins->i_tab[ind] != PIPE)
			ft_execve(cmd[0], cmd, ins, 1);
		if (ind && ins->i_tab[ind - 1] != PIPE && ins->i_tab[ind] == PIPE)
			ft_execve(cmd[0], cmd, ins, 2);
		if (ind && ins->i_tab[ind - 1] == PIPE && ins->i_tab[ind] == PIPE)
			ft_execve(cmd[0], cmd, ins, 3);
	}
	else
		ft_execve(cmd[0], cmd, ins, 0);
}

void	exe_command(char *command, char **envp, t_instruct *ins)
{
	char	**paths;
	char	**cmd;
	char	*cmd_err;

	cmd = ft_split(command, ' ');
	if (!exe_builtin(cmd, envp))
		return ;
	cmd_err = ft_strdup(cmd[0]);
	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
		call_ft_execve(cmd, ins);
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
			call_ft_execve(cmd, ins);
	}
}

int	is_valid_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (0);
	return (1);
}

void	parse_buffer(char *buffer, char **envp, t_instruct *instruct)
{
	int		i;
	int		start;
	char	*tmp;

	i = -1;
	start = 0;
	instruct->ind = -1;
	(void)instruct;
	while (buffer[++i])
	{
		if (!is_valid_char(buffer[i]))
		{
			instruct->ind++;
			tmp = ft_substr(buffer, start, i - start);
			start = i + 1;
			while (!is_valid_char(buffer[++i]) || buffer[i] == ' ')
				start++;
			exe_command(tmp, envp, instruct);
			free(tmp);
		}
	}
	if (start != i)
	{
		instruct->ind++;
		tmp = ft_substr(buffer, start, i);
		exe_command(tmp, envp, instruct);
		free(tmp);
	}
}
