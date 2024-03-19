/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:59:57 by ccadoret          #+#    #+#             */
/*   Updated: 2024/03/19 12:59:57 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_varcpy(int size_path, int ind, char **envp, char *var)
{
	int	i;
	int	j;

	i = size_path - 1;
	j = -1;
	while (envp[ind][++i])
	{
		if (envp[ind][i] == '"')
		{
			while (envp[ind][++i] != '"')
				var[++j] = envp[ind][i];
			i++;
		}
		var[++j] = envp[ind][i];
	}
	var[++j] = '\0';
}

char	*alloc_varenv(char *path, char **envp)
{
	int		i;
	int		j;
	int		size;
	char	*str;

	i = -1;
	size = 0;
	while (envp[++i])
		if (!ft_strncmp(path, envp[i], ft_strlen(path)))
		{
			j = ft_strlen(path) - 1;
			while (envp[i][++j])
			{
				if (envp[i][j] == '"')
				{
					while (envp[i][++j] != '"')
						size++;
					j++;
				}
				size++;
			}
		}
	str = malloc((size + 1) * sizeof(char));
	if (!str)
		return (NULL);
	return (str);
}

char	*ft_getenv(char *var, char **envp)
{
	char	*path;
	char	*tmp;
	int		i;

	path = ft_strjoin(var, "=", 0);
	i = -1;
	tmp = alloc_varenv(path, envp);
	while (envp[++i])
	{
		if (!ft_strncmp(path, envp[i], ft_strlen(path)))
		{
			ft_varcpy(ft_strlen(path), i, envp, tmp);
			return (free(path), tmp);
		}
	}
	return (free(path), NULL);
}

int	find_var(char **envp, char *var_name)
{
	int		i;
	int		len;
	char	*path;

	i = -1;
	path = ft_strjoin(var_name, "=", 0);
	len = ft_strlen(path);
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], path, len))
			return (free(path), i);
	}
	return (free(path), -1);
}

void	ft_unset(char **cmd, char **envp)
{
	int		indice;
	int		i;
	char	**tab;

	if (!cmd[1])
		ft_puterror("unset: not enough arguments\n");
	else if (ft_getenv(cmd[1], envp) != NULL)
	{
		indice = find_var(envp, cmd[1]);
		i = -1;
		while (envp[++i])
			continue ;
		tab = malloc(i * sizeof(char *));
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
