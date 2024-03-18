/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:28:28 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/18 10:03:25 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_pipes(t_instruct *ins, int close_before, int close_curr)
{
	int	i;

	i = -1;
	while (++i < ins->size)
	{
		if (i == ins->ind && !close_curr)
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

	(void)do_pipe;
	if (ins)
		envp = ins->envp;
	else
		envp = NULL;
	p = fork();
	if (p < 0)
		return (ft_puterror("fork: unable to create fork\n"), 1);
	if (p == 0)
	{
		dup_fds(ins, do_pipe);
		execve(path, argv, envp);
	}
	if (p > 0)
		waitpid(p, NULL, 0);
	return (0);
}

void	call_ft_execve(char **cmd, t_instruct *ins)
{
	int	ind;

	if (ins)
		ind = ins->ind;
	if (ins->ind > ins->size || ins->ind < 0)
		return ;
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
	if (!cmd || !exe_builtin(cmd, envp))
		return ;
	cmd_err = ft_strdup(cmd[0]);
	if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/')
		|| (cmd[0][0] == '~' && cmd[0][1] == '/'))
		call_ft_execve(cmd, ins);
	else
	{
		paths = get_paths(envp);
		cmd[0] = try_path(paths, cmd[0]);
		tab_free(paths);
		if (!cmd[0])
			ft_putferror("%s: command not found\n", cmd_err);
		else
			call_ft_execve(cmd, ins);
	}
	tab_free(cmd);
	free(cmd_err);
}
