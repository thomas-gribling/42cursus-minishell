/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 08:48:08 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/26 08:58:45 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo_fork(char **cmd)
{
	int	i;
	int	newline;

	newline = 1;
	i = 0;
	if (cmd[1] && !ft_strcmp(cmd[1], "-n"))
	{
		newline = 0;
		i++;
	}
	while (cmd[++i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
	}
	if (newline)
		printf("\n");
}

void	ft_echo(char **cmd, t_instruct *ins)
{
	ins->p = fork();
	if (ins->p < 0)
	{
		ft_putferror(ERR_CREATE, "fork", NULL, 0);
		return ;
	}
	if (ins->p > 0)
		return ;
	call_builtin(ins);
	ft_echo_fork(cmd);
	exit(0);
}

void	ft_cd(char **cmd, int *st)
{
	DIR	*dest_dir;

	if (cmd[1] && cmd[2])
		ft_putferror(ERR_TMARGS, "cd", st, 1);
	else if (!cmd[1])
		chdir(ft_getenv("HOME"));
	else if (!ft_strcmp(cmd[1], ""))
		return ;
	else if (cmd[1])
	{
		dest_dir = opendir(cmd[1]);
		if (access(cmd[1], F_OK))
			ft_putferror(ERR_NOFILE, cmd[1], st, 1);
		else if (!dest_dir)
			ft_putferror(ERR_NODIRPERM, cmd[1], st, 1);
		else
			chdir(cmd[1]);
		if (dest_dir)
			closedir(dest_dir);
		update_pwd();
	}
}
