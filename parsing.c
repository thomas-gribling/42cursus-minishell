/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:36:14 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/25 08:54:42 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_char(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
		return (0);
	return (1);
}

int	parse_buffer(char *buffer, t_instruct *instruct, int *i, int *st)
{
	char	*tmp;
	int		start;
	int		opened;

	start = 0;
	while (buffer[++(*i)])
	{
		if (buffer[*i] == '\'' || buffer[*i] == '"')
		{
			opened = buffer[(*i)];
			while (buffer[++(*i)] && buffer[*i] != opened)
				continue ;
		}
		if (!is_valid_char(buffer[*i]))
		{
			instruct->ind++;
			tmp = ft_substr(buffer, start, *i - start);
			start = *i + 1;
			while (!is_valid_char(buffer[++(*i)]) || buffer[*i] == ' ')
				start++;
			exe_command(tmp, instruct, st);
			free(tmp);
		}
	}
	return (start);
}

void	replace_var(char *old, char **new, int *i, int *st)
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
	return (new);
}

void	replace_root(char **buffer)
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
	return (new);
}

void	start_parsing(char *buffer, t_instruct *instruct, int *st)
{
	int		i;
	int		start;
	char	*tmp;

	i = -1;
	instruct->ind = -1;
	buffer = replace_vars(instruct, buffer, st);
	buffer = replace_roots(buffer);
	start = parse_buffer(buffer, instruct, &i, st);
	if (start != i)
	{
		instruct->ind++;
		tmp = ft_substr(buffer, start, i);
		exe_command(tmp, instruct, st);
		free(tmp);
	}
	close_all_pipes(instruct, 1, 0);
	if (strncmp(buffer, "cd", 2) && strncmp(buffer, "unset", 5))
		waitpid(instruct->p, st, 0);
}
