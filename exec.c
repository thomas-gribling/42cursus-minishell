/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:28:28 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/02 16:49:53 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	exe_command_path(char **cmd, t_instruct *ins, char *cmd_err, int *st)
{
	char	**paths;

	paths = get_paths();
	cmd[0] = try_path(paths, cmd[0]);
	tab_free(paths);
	if (!cmd[0])
		ft_putferror(ERR_NOCMD, cmd_err, st, 127);
	else
		call_ft_execve(cmd, ins);
}

void	exe_command(char **cmd, t_instruct *ins, int *st)
{
	char	*cmd_err;

	if (!cmd || !exe_builtin(ins, cmd, st))
		return ;
	cmd_err = ft_strdup(cmd[0]);
	if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/')
		|| (cmd[0][0] == '~' && cmd[0][1] == '/'))
	{
		if (access(cmd[0], F_OK))
			ft_putferror(ERR_NOFILE, cmd_err, st, 127);
		else if ((access(cmd[0], X_OK)))
			ft_putferror(ERR_NOPERM, cmd_err, st, 126);
		else
			call_ft_execve(cmd, ins);
	}
	else
		exe_command_path(cmd, ins, cmd_err, st);
	tab_free(cmd);
	free(cmd_err);
}
