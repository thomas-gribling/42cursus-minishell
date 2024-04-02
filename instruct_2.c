/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instruct_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:09:43 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/02 17:16:02 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipes_init(t_instruct *instruct)
{
	int	i;

	i = -1;
	instruct->pipes = malloc(sizeof(int *) * instruct->size);
	while (++i < instruct->size)
	{
		instruct->pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(instruct->pipes[i]) < 0)
			ft_putferror(ERR_CREATE, "pipe", NULL, 0);
	}
}

int	is_redirect_first(char *str, int i)
{
	if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return (HEREDOC_I);
		return (REDIRECT_I);
	}
	if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return (HEREDOC_O);
		return (REDIRECT_O);
	}
	return (0);
}

int	is_var_valid(char *str, int i, int opened)
{
	int	j;
	int	counter;

	if (opened == '\'')
		return (0);
	counter = 0;
	j = i;
	while (--j >= 0)
	{
		if (str[j] == '$')
			counter++;
		else
			break ;
	}
	if (counter % 2)
		return (0);
	i++;
	if (!str[i] || str[i] == ' ' || str[i] == '\'' || str[i] == '"')
		return (0);
	return (1);
}

void	get_quotes(char c, int *opened)
{
	if (c == '"' || c == '\'')
	{
		if (!(*opened))
			*opened = c;
		else if (*opened == c)
			*opened = 0;
	}
}

void	vars_init(t_instruct *instruct, char *str)
{
	int	i;
	int	count;
	int	opened;

	count = 0;
	opened = 0;
	i = -1;
	while (str[++i])
		if (str[i] == '$')
			count++;
	instruct->var_tab = malloc(count * sizeof(int));
	i = -1;
	count = -1;
	while (str[++i])
	{
		get_quotes(str[i], &opened);
		if (str[i] == '$')
		{
			if (is_var_valid(str, i, opened))
				instruct->var_tab[++count] = 1;
			else
				instruct->var_tab[++count] = 0;
		}
	}
}
