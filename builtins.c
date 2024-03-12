/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:05:03 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/12 15:24:56 by tgriblin         ###   ########.fr       */
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

int	exe_builtin(char **cmd, char **envp)
{
	(void)envp;
	if (!ft_strcmp(cmd[0], "pwd"))
		return (ft_pwd_env(0, NULL), 0);
	if (!ft_strcmp(cmd[0], "env"))
		return (ft_pwd_env(1, envp), 0);
	if (!ft_strcmp(cmd[0], "echo"))
		return (ft_echo(cmd), 0);
	if (!ft_strcmp(cmd[0], "cd"))
		return (printf("builtin command!\n"), 0);
	if (!ft_strcmp(cmd[0], "export")
		|| !ft_strcmp(cmd[0], "unset"))
	{
		return (printf("builtin command!\n"), 0);	
	}
	return (1);
}