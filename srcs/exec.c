/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:28:28 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/05 08:58:13 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_execve(char *path, char **argv, t_instruct *ins, int do_pipe)
{
	ins->p = fork();
	if (ins->p < 0)
		return (ft_putferror(ERR_CREATE, "fork", NULL, 0), 1);
	if (ins->p == 0)
	{
		dup_fds(ins, do_pipe);
		execve(path, argv, g_envp);
		exit(1);
	}
	return (0);
}

int	call_ft_execve(char **cmd, t_instruct *ins)
{
	int	ind;

	if (ins)
		ind = ins->ind;
	if (ins->ind > ins->size || ins->ind < 0)
		return (0);
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
	return (1);
}

int	exe_command_path(char **cmd, t_instruct *ins, char *cmd_err, int *st)
{
	char	**paths;

	paths = get_paths();
	cmd[0] = try_path(paths, cmd[0]);
	tab_free(paths);
	if (!cmd[0])
	{
		cmd[0] = ft_strdup("foo");
		return (ft_putferror(ERR_NOCMD, cmd_err, st, 127), 0);
	}
	else
		call_ft_execve(cmd, ins);
	return (1);
}

int	exe_command(char **cmd, t_instruct *ins, int *st)
{
	char	*cmd_err;
	int		out;

	out = exe_builtin(ins, cmd, st);
	if (!cmd || out != -1)
		return (out);
	out = 0;
	cmd_err = ft_strdup(cmd[0]);
	if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/')
		|| (cmd[0][0] == '~' && cmd[0][1] == '/'))
	{
		out = 0;
		if (access(cmd[0], F_OK))
			ft_putferror(ERR_NOFILE, cmd_err, st, 127);
		else if ((access(cmd[0], X_OK)))
			ft_putferror(ERR_NOPERM, cmd_err, st, 126);
		else
			out = call_ft_execve(cmd, ins);
	}
	else
		out = exe_command_path(cmd, ins, cmd_err, st);
	tab_free(cmd);
	free(cmd_err);
	return (out);
}

void	exe_command_quick(char *command, char **paths, char *cmd)
{
	pid_t	p;

	paths = get_paths();
	if (!paths)
		return ;
	cmd = ft_strdup(command);
	cmd = try_path(paths, cmd);
	tab_free(paths);
	paths = malloc(2 * sizeof(char *));
	paths[0] = ft_strdup(cmd);
	paths[1] = NULL;
	if (!cmd)
		return ;
	else
	{
		p = fork();
		if (p < 0)
			ft_putferror(ERR_CREATE, "fork", NULL, 0);
		if (p == 0)
			execve(cmd, paths, g_envp);
		if (p > 0)
			waitpid(p, NULL, 0);
	}
	tab_free(paths);
	free(cmd);
}
