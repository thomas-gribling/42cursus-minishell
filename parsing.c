/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:36:14 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/18 14:37:48 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_char(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
		return (0);
	return (1);
}

int	parse_buffer(char *buffer, char **envp, t_instruct *instruct, int *i)
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
			exe_command(tmp, envp, instruct);
			free(tmp);
		}
	}
	return (start);
}

void	replace_var(char *old, char *new, int *i)
{
	int		start;
	char	*temp1;
	char	*temp2;

	start = *i;
	while (old[++(*i)] != '$' && old[*i] != '\''
		&& old[*i] != '"' && old[*i] != ' ' && old[*i])
		continue ;
	temp1 = ft_substr(old, start + 1, *i - start - 1);
	temp2 = getenv(temp1);
	free(temp1);
	if (temp2)
	{
		start = -1;
		while (temp2[++start])
			new = str_append(new, temp2[start]);
	}
	(*i)--;
}

char	*replace_vars(t_instruct *ins, char *old)
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
			replace_var(old, new, &i);
		else
			new = str_append(new, old[i]);
	}
	return (new);
}

void	start_parsing(char *buffer, char **envp, t_instruct *instruct)
{
	int		i;
	int		start;
	char	*tmp;

	i = -1;
	instruct->ind = -1;
	buffer = replace_vars(instruct, buffer);
	start = parse_buffer(buffer, envp, instruct, &i);
	if (start != i)
	{
		instruct->ind++;
		tmp = ft_substr(buffer, start, i);
		exe_command(tmp, envp, instruct);
		free(tmp);
	}
}
