/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:31:23 by ccadoret          #+#    #+#             */
/*   Updated: 2024/04/03 15:56:09 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	*red_valid(char *s, int size)
{
	int	*out;
	int	i;
	int	j;
	int	opened;

	out = malloc((size + 1) * sizeof(int));
	if (!out)
		return (NULL);
	opened = 0;
	j = -1;
	i = -1;
	while (s[++i])
	{
		if ((s[i] == '"' || s[i] == '\'') && !opened)
			opened = s[i];
		else if ((s[i] == '"' || s[i] == '\'') && opened == s[i])
			opened = 0;
		if (is_it_redirect(s, &i))
			out[++j] = !opened;
	}
	out[++j] = -1;
	return (out);
}

static int	count_valid_redirects(char *buffer)
{
	int	opened;
	int	i;
	int	count;

	i = -1;
	count = 0;
	opened = 0;
	while (buffer[++i])
	{
		if (buffer[i] == '\'' || buffer[i] == '"')
		{
			if (!opened)
				opened = buffer[i];
			else if (opened == buffer[i])
				opened = 0;
		}
		if ((buffer[i] == '<' || buffer[i] == '>') && !opened)
			count++;
	}
	return (count);
}

int	is_redirect(t_instruct *ins, char *command, int *st)
{
	t_redirect	red;
	int			i;

	red.cv = count_valid_redirects(command);
	if (!red.cv)
		return (0);
	i = -1;
	red.count = 0;
	while (command[++i])
		if (is_it_redirect(command, &i))
			red.count++;
	red.validity = red_valid(command, red.count);
	if (!red.validity)
		return (0);
	fill_redir(ins, command, st, &red);
	return (free(red.validity), 1);
}
