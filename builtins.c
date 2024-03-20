/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:05:03 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/20 14:55:00 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_pwd(char *arg)
{
	char	*path;
	
	if (arg)
		return (ft_putferror(ERR_TMARGS, "pwd"), 1);
	else
	{
		path = getcwd(NULL, 0);
		printf("%s\n", path);
		free(path);
	}
	return (0);
}

void	ft_pwd_env(char **cmd, char **envp, t_instruct *ins)
{
	int		i;

	ins->p = fork();
	if (ins->p < 0)
	{
		ft_putferror(ERR_CREATE, "fork");
		return ;
	}
	if (ins->p > 0)
		return ;
	call_builtin(ins);
	i = -1;
	if (!ft_strcmp(cmd[0], "pwd"))
		exit(ft_pwd(cmd[1]));
	else if (cmd[1])
	{
		ft_putferror(ERR_TMARGS, "env");
		exit(1);
	}
	else
		while (envp[++i])
			printf("%s\n", envp[i]);
	exit(0);
}

void	ft_echo(char **cmd, t_instruct *ins)
{
	int	newline;
	int	i;

	ins->p = fork();
	if (ins->p < 0)
	{
		ft_putferror(ERR_CREATE, "fork");
		return ;
	}
	if (ins->p > 0)
		return ;
	call_builtin(ins);
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
	exit(0);
}

void	ft_cd(char **cmd, char **envp)
{
	DIR	*dest_dir;

	if (cmd[1] && cmd[2])
		ft_putferror(ERR_TMARGS, "cd");
	else if (!cmd[1])
		chdir(ft_getenv("HOME", envp));
	else if (!ft_strcmp(cmd[1], ""))
		return ;
	else if (cmd[1])
	{
		dest_dir = opendir(cmd[1]);
		if (access(cmd[1], F_OK))
			ft_putferror(ERR_NOFILE, cmd[1]);
		else if (!dest_dir)
			ft_putferror(ERR_NODIRPERM, cmd[1]);
		else
			chdir(cmd[1]);
		if (dest_dir)
			closedir(dest_dir);
		update_pwd(envp);
	}
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

int	exe_builtin(t_instruct *ins, char **cmd, char **envp)
{
	(void)ins;
	if (!is_builtin(cmd[0]))
		return (1);
	if (!ft_strcmp(cmd[0], "pwd") || !ft_strcmp(cmd[0], "env"))
		ft_pwd_env(cmd, envp, ins);
	if (!ft_strcmp(cmd[0], "echo"))
		ft_echo(cmd, ins);
	if (!ft_strcmp(cmd[0], "cd"))
		ft_cd(cmd, envp);
	if (!ft_strcmp(cmd[0], "unset"))
		ft_unset(cmd, envp);
	if (!ft_strcmp(cmd[0], "export"))
		ft_export(envp, cmd[1]);
	tab_free(cmd);
	return (0);
}
