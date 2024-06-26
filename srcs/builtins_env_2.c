/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:26:57 by ccadoret          #+#    #+#             */
/*   Updated: 2024/04/08 10:21:50 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	update_pwd(void)
{
	int		indice[2];
	char	*tmp;

	indice[0] = find_var("PWD");
	indice[1] = find_var("OLDPWD");
	if (indice[0] >= 0 && indice[1] >= 0)
	{
		tmp = ft_getenv("PWD");
		if (!tmp)
			return ;
		free(g_envp[indice[1]]);
		g_envp[indice[1]] = ft_strjoin("OLDPWD=", tmp, 2);
	}
	if (indice[0] >= 0)
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
			return ;
		free(g_envp[indice[0]]);
		g_envp[indice[0]] = ft_strjoin("PWD=", tmp, 2);
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
