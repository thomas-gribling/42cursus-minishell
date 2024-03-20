/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:26:57 by ccadoret          #+#    #+#             */
/*   Updated: 2024/03/20 10:26:57 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(char **envp)
{
	int		indice;

	indice = find_var(envp, "OLDPWD");
	if (indice >= 0)
	{
		free(envp[indice]);
		envp[indice] = ft_strjoin("OLDPWD=", ft_getenv("PWD", envp), 2);
	}
	indice = find_var(envp, "PWD");
	if (indice >= 0)
	{
		free(envp[indice]);
		envp[indice] = ft_strjoin("PWD=", getcwd(NULL, 0), 2);
	}
}

void	ft_export(char **envp, char *new_var)
{
	int		i;
	char	**new_envp;

	i = -1;
	while (envp[++i])
		continue ;
	new_envp = malloc((i + 2) * sizeof(char *));
	i = -1;
	while (envp[++i])
		new_envp[i] = ft_strdup(envp[i]);
	new_envp[i] = ft_strdup(new_var);
	new_envp[i] = NULL;
	tab_free(envp);
	envp = new_envp;
}
