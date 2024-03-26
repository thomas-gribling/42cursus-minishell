/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instruct.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:50:29 by ccadoret          #+#    #+#             */
/*   Updated: 2024/03/12 14:50:29 by ccadoret         ###   ########.fr       */
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
		if (str[i] == '\'' || str[i] == '"')
		{
			if (!opened)
				opened = str[i];
			else if (opened == str[i])
				opened = 0;
		}
		if (str[i] == '$')
		{
			if (is_var_valid(str, i, opened))
				instruct->var_tab[++count] = 1;
			else
				instruct->var_tab[++count] = 0;
		}
	}
}

t_instruct	init_tabinstruct(char *str)
{
	t_instruct	tab;
	int			i;
	int			j;

	tab.size = 0;
	if (!str)
		return (tab);
	i = -1;
	while (str[++i])
		if (str[i] == '|')
			tab.size++;
	tab.i_tab = malloc(sizeof(int) * (tab.size + 1));
	j = -1;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '|')
			tab.i_tab[++j] = PIPE;
	}
	tab.i_tab[++j] = 0;
	pipes_init(&tab);
	vars_init(&tab, str);
	return (tab);
}

void	free_instruct(t_instruct *tab)
{
	int	i;

	free(tab->i_tab);
	free(tab->var_tab);
	close_all_pipes(tab, 1, 1);
	i = -1;
	while (++i < tab->size)
		free(tab->pipes[i]);
	free(tab->pipes);
}

int	check_string(char *str, int i, char opened, int *st)
{
	while (str[++i])
	{
		if (opened || str[i] == '\'' || str[i] == '"')
		{
			if (!opened)
				opened = str[i];
			else if (str[i] == opened)
				opened = 0;
		}
		if (str[i] == '|' && !opened)
		{
			while (str[++i] == ' ')
				continue ;
			if (str[i] == '|' || !str[i])
				return (ft_putferror(ERR_PARSE, "|", st, 1), 0);
		}
		if (str[i] == '&' && !opened)
			return (ft_putferror(ERR_PARSE, "&", st, 1), 0);
	}
	if (opened)
		return (ft_putferror(ERR_UNCLOSED, "quotes", st, 1), 0);
	return (1);
}

int	verif_instruct(char *str, int *st)
{
	int	i;
	int	opened;

	i = -1;
	opened = 0;
	while (str[++i] == ' ')
		continue ;
	if (str[i] == '|')
		return (ft_putferror(ERR_PARSE, "|", st, 1), 0);
	if (str[i] == '&')
		return (ft_putferror(ERR_PARSE, "&", st, 1), 0);
	if ((str[i] == '\'' || str[i] == '"') && !opened)
		opened = str[i];
	return (check_string(str, i, opened, st));
}
