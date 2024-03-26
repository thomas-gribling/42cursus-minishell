/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:59:57 by ccadoret          #+#    #+#             */
/*   Updated: 2024/03/26 09:00:44 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_varcpy(int size_path, int ind, char *var)
{
	int	i;
	int	j;

	i = size_path - 1;
	j = -1;
	while (g_envp[ind][++i])
	{
		if (g_envp[ind][i] == '"')
		{
			while (g_envp[ind][++i] != '"')
				var[++j] = g_envp[ind][i];
			i++;
		}
		var[++j] = g_envp[ind][i];
	}
	var[++j] = '\0';
}

char	*alloc_varenv(char *path)
{
	int		i;
	int		j;
	int		size;
	char	*str;

	i = -1;
	size = 0;
	while (g_envp[++i])
	{
		if (!ft_strncmp(path, g_envp[i], ft_strlen(path)))
		{
			j = ft_strlen(path) - 1;
			while (g_envp[i][++j])
			{
				if (g_envp[i][j] == '"')
				{
					while (g_envp[i][++j] != '"')
						size++;
					j++;
				}
				size++;
			}
		}
	}
	str = malloc((size + 1) * sizeof(char));
	if (!str)
		return (NULL);
	return (str);
}

char	*ft_getenv(char *var)
{
	char	*path;
	char	*tmp;
	int		i;

	path = ft_strjoin(var, "=", 0);
	i = -1;
	tmp = alloc_varenv(path);
	while (g_envp[++i])
	{
		if (!ft_strncmp(path, g_envp[i], ft_strlen(path)))
		{
			ft_varcpy(ft_strlen(path), i, tmp);
			return (free(path), tmp);
		}
	}
	return (free(path), NULL);
}

int	find_var(char *var_name)
{
	int		i;
	int		len;
	char	*path;

	i = -1;
	path = ft_strjoin(var_name, "=", 0);
	len = ft_strlen(path);
	while (g_envp[++i])
	{
		if (!ft_strncmp(g_envp[i], path, len))
			return (free(path), i);
	}
	return (free(path), -1);
}

void	ft_unset(char *cmd, int *st)
{
	int		indice;
	int		i;
	char	**tab;

	if (!cmd)
		ft_putferror(ERR_NEARGS, "unset", st, 1);
	else if (ft_getenv(cmd) != NULL)
	{
		indice = find_var(cmd);
		i = -1;
		while (g_envp[++i])
			continue ;
		tab = malloc(i * sizeof(char *));
		i = -1;
		while (++i < indice)
			tab[i] = ft_strdup(g_envp[i]);
		while (g_envp[++i])
			tab[i - 1] = ft_strdup(g_envp[i]);
		tab[i - 1] = NULL;
		tab_free(g_envp);
		g_envp = tab;
	}
}
