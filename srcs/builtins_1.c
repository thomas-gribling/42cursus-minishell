/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:05:03 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/03 15:54:55 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	call_builtin(t_instruct *ins)
{
	int	ind;

	if (ins)
		ind = ins->ind;
	if (ins->ind > ins->size || ins->ind < 0)
		return ;
	if (ins && ind <= ins->size)
	{
		if (!ind && !ins->size)
			dup_fds(ins, 0);
		if (!ind && ins->i_tab[ind] == PIPE)
			dup_fds(ins, 2);
		if (ind && ins->i_tab[ind - 1] == PIPE && ins->i_tab[ind] != PIPE)
			dup_fds(ins, 1);
		if (ind && ins->i_tab[ind - 1] != PIPE && ins->i_tab[ind] == PIPE)
			dup_fds(ins, 2);
		if (ind && ins->i_tab[ind - 1] == PIPE && ins->i_tab[ind] == PIPE)
			dup_fds(ins, 3);
	}
	else
		dup_fds(ins, 0);
}

int	ft_pwd(char *arg, int *st)
{
	char	*path;

	if (arg)
		return (ft_putferror(ERR_TMARGS, "pwd", st, 1), 1);
	else
	{
		path = getcwd(NULL, 0);
		printf("%s\n", path);
		free(path);
	}
	return (0);
}

void	ft_pwd_env(char **cmd, t_instruct *ins, int *st)
{
	int		i;

	ins->p = fork();
	if (ins->p < 0)
	{
		ft_putferror(ERR_CREATE, "fork", NULL, 0);
		return ;
	}
	if (ins->p > 0)
		return ;
	call_builtin(ins);
	i = -1;
	if (!ft_strcmp(cmd[0], "pwd"))
		exit(ft_pwd(cmd[1], st));
	else if (cmd[1])
	{
		ft_putferror(ERR_TMARGS, "env", NULL, 0);
		exit(1);
	}
	else
		while (g_envp[++i])
			printf("%s\n", g_envp[i]);
	exit(0);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "pwd"))
		if (ft_strcmp(cmd, "env"))
			if (ft_strcmp(cmd, "echo"))
				if (ft_strcmp(cmd, "cd"))
					if (ft_strcmp(cmd, "unset"))
						if (ft_strcmp(cmd, "export"))
							return (0);
	return (1);
}

int	exe_builtin(t_instruct *ins, char **cmd, int *st)
{
	if (!is_builtin(cmd[0]))
		return (1);
	*st = 0;
	if (!ft_strcmp(cmd[0], "pwd") || !ft_strcmp(cmd[0], "env"))
		ft_pwd_env(cmd, ins, st);
	if (!ft_strcmp(cmd[0], "echo"))
		ft_echo(cmd, ins);
	if (!ft_strcmp(cmd[0], "cd"))
		ft_cd(cmd, st);
	if (!ft_strcmp(cmd[0], "unset") || !ft_strcmp(cmd[0], "export"))
		ft_unset_export(cmd, st, ins);
	tab_free(cmd);
	return (0);
}
