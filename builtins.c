/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:05:03 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/19 14:15:28 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd_env(char **cmd, char **envp)
{
	int		i;
	char	*path;

	if (!ft_strcmp(cmd[0], "pwd"))
	{
		if (cmd[1])
			ft_puterror("pwd: too many arguments\n");
		else
		{
			path = getcwd(NULL, 0);
			printf("%s\n", path);
			free(path);
		}
	}
	else
	{
		i = -1;
		while (envp[++i])
			printf("%s\n", envp[i]);
	}
}

void	ft_echo(char **cmd)
{
	int	newline;
	int	i;

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

char	*replace_root(char *old)
{
	int		i;
	int		j;
	char	*new;
	char	*home;

	i = -1;
	new = NULL;
	home = getenv("HOME");
	if (old[0] == '~')
	{
		j = -1;
		while (home[++j])
			new = str_append(new, home[j]);
		i++;
	}
	while (old[++i])
		new = str_append(new, old[i]);
	free(old);
	return (new);
}

void	ft_cd(char **cmd)
{
	DIR	*dest_dir;

	if (!cmd[1])
		chdir(getenv("HOME"));
	else if (!ft_strcmp(cmd[1], ""))
		return ;
	else if (cmd[1])
	{
		cmd[1] = replace_root(cmd[1]);
		dest_dir = opendir(cmd[1]);
		if (access(cmd[1], F_OK))
			ft_putferror("%s: no such file or directory\n", cmd[1]);
		else if (!dest_dir)
			ft_putferror("%s: not a directory or permission denied\n", cmd[1]);
		else
			chdir(cmd[1]);
		if (dest_dir)
			closedir(dest_dir);
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

int	exe_builtin(t_instruct *ins, char **cmd, char **envp)
{
	if (!is_builtin(cmd[0]))
		return (1);
	ins->p = fork();
	if (ins->p < 0)
		return (ft_puterror("fork: unable to create fork\n"), 1);
	if (ins->p == 0)
	{
		call_builtin(ins);
		if (!ft_strcmp(cmd[0], "pwd") || !ft_strcmp(cmd[0], "env"))
			ft_pwd_env(cmd, envp);
		if (!ft_strcmp(cmd[0], "echo"))
			ft_echo(cmd);
		if (!ft_strcmp(cmd[0], "cd"))
			ft_cd(cmd);
		tab_free(cmd);
		exit(0);
	}
	if (ins->p > 0)
	{
		if (!ft_strcmp(cmd[0], "unset"))
			ft_unset(cmd, envp);
		if (!ft_strcmp(cmd[0], "export"))
			printf("TODO\n");
	}
	return (0);
}
