/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:36:14 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/03 15:55:48 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_valid_char(char c)
{
	if (c == '|' || c == '&')
		return (0);
	return (1);
}

static void	skip_quotes(char *buffer, int *opened, int *i)
{
	*opened = buffer[(*i)];
	while (buffer[++(*i)] && buffer[*i] != *opened)
		continue ;
}

static int	do_waitpid(char *buffer)
{
	int	i;
	int	len;

	if (!ft_strncmp(buffer, "cd", 2) || !ft_strncmp(buffer, "unset", 5))
		return (0);
	i = -1;
	len = 0;
	while (buffer[++i])
		if (buffer[i] != ' ')
			len++;
	if (!ft_strncmp(buffer, "export", 6) && len > 6)
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
			skip_quotes(buffer, &opened, i);
		if (!is_valid_char(buffer[*i]))
		{
			instruct->ind++;
			tmp = ft_substr(buffer, start, *i - start);
			start = *i + 1;
			while (!is_valid_char(buffer[++(*i)]) || buffer[*i] == ' ')
				start++;
			if (!is_redirect(instruct, tmp, st))
				exe_command(ft_split(tmp, ' '), instruct, st);
			free(tmp);
		}
	}
	return (start);
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
		if (!is_redirect(instruct, tmp, st))
			exe_command(ft_split(tmp, ' '), instruct, st);
		free(tmp);
	}
	close_all_pipes(instruct, 1, 0);
	if (do_waitpid(buffer))
		waitpid(instruct->p, st, 0);
}
