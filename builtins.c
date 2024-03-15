/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:05:03 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/15 14:13:02 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd_env(int env, char **envp)
{
	int		i;
	char	*path;

	if (!env)
	{
		path = getcwd(NULL, 0);
		printf("%s\n", path);
		free(path);
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

void	ft_cd(char **cmd)
{
	DIR	*dest_dir;

	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
		chdir(getenv("HOME"));
	else if (cmd[1])
	{
		dest_dir = opendir(cmd[1]);
		if (access(cmd[1], F_OK))
			ft_putferror("%s: no such file or directory\n", cmd[1]);
		else if (!dest_dir)
			ft_putferror("%s: not a directory\n", cmd[1]);
		else
			chdir(cmd[1]);
		if (dest_dir)
			closedir(dest_dir);
	}
}

void	ft_unset(char **cmd)
{
	if (!cmd[1])
		ft_puterror("unset: not enough arguments\n");
}

int	exe_builtin(char **cmd, char **envp)
{
	int	i;

	i = -1;
	while (cmd[++i])
		printf("[%s]", cmd[i]);
	printf("\n");
	if (!ft_strcmp(cmd[0], "pwd"))
		return (ft_pwd_env(0, NULL), tab_free(cmd), 0);
	if (!ft_strcmp(cmd[0], "env"))
		return (ft_pwd_env(1, envp), tab_free(cmd), 0);
	if (!ft_strcmp(cmd[0], "echo"))
		return (ft_echo(cmd), tab_free(cmd), 0);
	if (!ft_strcmp(cmd[0], "cd"))
		return (ft_cd(cmd), tab_free(cmd), 0);
	if (!ft_strcmp(cmd[0], "unset"))
		return (ft_unset(cmd), tab_free(cmd), 0);
	if (!ft_strcmp(cmd[0], "export"))
		return (printf("TODO\n"), tab_free(cmd), 0);
	return (1);
}
