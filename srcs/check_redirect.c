/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 13:20:43 by ccadoret          #+#    #+#             */
/*   Updated: 2024/04/03 15:55:13 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	valid_redirect(char *str)
{
	if (str[0] == '<' && str[1] == '<' && str[2] == ' ')
		return (HEREDOC_I);
	if (str[0] == '>' && str[1] == '>' && str[2] == ' ')
		return (HEREDOC_O);
	if (str[0] == '<' && str[1] == ' ')
		return (REDIRECT_I);
	if (str[0] == '>' && str[1] == ' ')
		return (REDIRECT_O);
	return (0);
}

static void	skip_quotes(char *str, int *i)
{
	if (str[*i] == '"')
		while (str[++(*i)] != '"' && str[*i])
			continue ;
	if (str[*i] == '\'')
		while (str[++(*i)] != '\'' && str[*i])
			continue ;
}

int	check_redirect(char *str, int *st)
{
	int		i;
	int		last;

	i = -1;
	last = -1;
	while (str[++i])
	{
		skip_quotes(str, &i);
		if (str[i] == '<' || str[i] == '>')
		{
			if (!valid_redirect(str + i) || valid_redirect(str + i) == last)
				return (ft_putferror(ERR_PARSE, "redirection", st, 1), 0);
			last = valid_redirect(str + i);
			if (last == HEREDOC_I || last == HEREDOC_O)
				i++;
			while (str[++i] == ' ')
				continue ;
			if (str[i] == '<' || str[i] == '>')
				return (ft_putferror(ERR_PARSE, "redirections", st, 1), 0);
		}
	}
	return (1);
}
