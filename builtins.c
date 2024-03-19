/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:05:03 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/19 10:13:26 by tgriblin         ###   ########.fr       */
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

char	*ft_getenv(char *var, char **envp)
{
	char	*path;
	char	*tmp;
	int		i;
	int		j;
	int		k;

	path = ft_strjoin(var, "=", 0);
	i = -1;
	k = ft_strlen(path);
	tmp = malloc(sizeof(int)); // TODO
	while (envp[++i])
	{
		if (!ft_strncmp(path, envp[i], k))
		{
			j = k - 1;
			while (envp[i][++j])
			{
				if (envp[i][j] == '"')
					while (envp[i][++j] != '"')
						tmp[i - k - 1] = envp[i][j];
				tmp[i - k - 1] = envp[i][j];
			}
			return (free(path), tmp);
		}
	}
	return (free(path), NULL);
}

int	find_var(char **envp, char *var_name)
{
	int	i;
	int	len;

	i = -1;
	len = ft_strlen(var_name);
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], var_name, len))
			return (i);
	}
	return (-1);
}

void	ft_unset(char **cmd, char **envp)
{
	int		indice;
	int		i;
	char	**tab;

	if (!cmd[1])
		ft_puterror("unset: not enough arguments\n");
	if (getenv(cmd[1]))
	{
		indice = find_var(envp, cmd[1]);
		i = -1;
		while (envp[++i])
			continue ;
		tab = malloc (i * sizeof(char));
		i = -1;
		while (++i < indice)
			tab[i] = ft_strdup(envp[i]);
		while (envp[++i])
			tab[i - 1] = ft_strdup(envp[i]);
		tab[i - 1] = NULL;
		tab_free(envp);
		envp = tab;
	}
}

int	exe_builtin(char **cmd, char **envp)
{
	if (!ft_strcmp(cmd[0], "pwd") || !ft_strcmp(cmd[0], "env"))
		return (ft_pwd_env(cmd, envp), tab_free(cmd), 0);
	if (!ft_strcmp(cmd[0], "echo"))
		return (ft_echo(cmd), tab_free(cmd), 0);
	if (!ft_strcmp(cmd[0], "cd"))
		return (ft_cd(cmd), tab_free(cmd), 0);
	if (!ft_strcmp(cmd[0], "unset"))
		return (ft_unset(cmd, envp), tab_free(cmd), 0);
	if (!ft_strcmp(cmd[0], "export"))
		return (printf("TODO\n"), tab_free(cmd), 0);
	return (1);
}
