/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:56:18 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/04 08:56:56 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	replace_var(char *old, char **new, int *i, int *st)
{
	int		start;
	char	*var_name;
	char	*var_value;

	start = *i;
	while (old[++(*i)] != '$' && old[*i] != '\''
		&& old[*i] != '"' && old[*i] != ' ' && old[*i])
		continue ;
	var_name = ft_substr(old, start + 1, *i - start - 1);
	if (!ft_strcmp(var_name, "?"))
		var_value = ft_itoa(*st);
	else
		var_value = ft_getenv(var_name);
	free(var_name);
	if (var_value)
	{
		start = -1;
		while (var_value[++start])
			*new = str_append(*new, var_value[start]);
	}
	free(var_value);
	(*i)--;
}

char	*replace_vars(t_instruct *ins, char *old, int *st)
{
	int		i;
	int		ind;
	char	*new;

	new = NULL;
	i = -1;
	ind = -1;
	while (old[++i])
	{
		if (old[i] == '$' && ins->var_tab[++ind])
			replace_var(old, &new, &i, st);
		else
			new = str_append(new, old[i]);
	}
	free(old);
	return (new);
}

static void	replace_root(char **buffer)
{
	int		j;
	char	*home;

	j = -1;
	home = ft_getenv("HOME");
	while (home[++j])
		*buffer = str_append(*buffer, home[j]);
	free(home);
}

char	*replace_roots(char *old)
{
	int		i;
	int		opened;
	char	*new;

	i = -1;
	opened = 0;
	new = NULL;
	while (old[++i])
	{
		if (old[i] == '\'' || old[i] == '"')
		{
			if (!opened)
				opened = old[i];
			else if (opened == old[i])
				opened = 0;
		}
		if (!opened && old[i] == '~' && (i == 0 || old[i - 1] == ' '))
			replace_root(&new);
		else
			new = str_append(new, old[i]);
	}
	free(old);
	return (new);
}
