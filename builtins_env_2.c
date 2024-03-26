/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:26:57 by ccadoret          #+#    #+#             */
/*   Updated: 2024/03/26 08:53:25 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(void)
{
	int		indice;

	indice = find_var("OLDPWD");
	if (indice >= 0)
	{
		free(g_envp[indice]);
		g_envp[indice] = ft_strjoin("OLDPWD=", ft_getenv("PWD"), 2);
	}
	indice = find_var("PWD");
	if (indice >= 0)
	{
		free(g_envp[indice]);
		g_envp[indice] = ft_strjoin("PWD=", getcwd(NULL, 0), 2);
	}
}

int	find_var_pref(char *new_var)
{
	int		i;
	int		indice;
	char	*var_name;

	i = -1;
	while (new_var[++i] && new_var[i] != '=')
		continue ;
	var_name = malloc(sizeof(char) * (i + 1));
	i = -1;
	while (new_var[++i] && new_var[i] != '=')
		var_name[i] = new_var[i];
	var_name[i] = '\0';
	indice = find_var(var_name);
	free(var_name);
	return (indice);
}

int	is_valid_var(char *cmd, int *st)
{
	int	i;

	i = -1;
	if (cmd[0] <= '9' && cmd[0] >= '0')
		return (ft_putferror(ERR_VARNAME, "export", st, 1), 0);
	if (cmd[0] == '=')
		return (ft_putferror(ERR_VARMISS, "export", st, 1), 0);
	while (cmd[++i])
		if (cmd[i] == '=')
			return (1);
	return (0);
}

void	ft_unset_export(char **cmd, int *st, t_instruct *ins)
{
	int	i;

	i = 0;
	if (cmd[0][0] == 'e')
	{
		if (!cmd[1])
		{
			ft_pwd_env(cmd, ins, st);
			return ;
		}
		while (cmd[++i])
		{
			if (is_valid_var(cmd[i], st))
				ft_export(cmd[i]);
		}
	}
	else
	{
		while (cmd[++i])
			ft_unset(cmd[i], st);
	}
}

void	ft_export(char *new_var)
{
	int		i;
	char	**new_envp;

	i = find_var_pref(new_var);
	if (i != -1)
	{
		free(g_envp[i]);
		g_envp[i] = ft_strdup(new_var);
	}
	else
	{
		i = -1;
		while (g_envp[++i])
			continue ;
		new_envp = malloc((i + 2) * sizeof(char *));
		i = -1;
		while (g_envp[++i])
			new_envp[i] = ft_strdup(g_envp[i]);
		new_envp[i] = ft_strdup(new_var);
		new_envp[++i] = NULL;
		tab_free(g_envp);
		g_envp = new_envp;
	}
}
