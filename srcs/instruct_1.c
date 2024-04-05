/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instruct_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:50:29 by ccadoret          #+#    #+#             */
/*   Updated: 2024/04/05 11:49:14 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_pipes_amt(char *str)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (str[++i])
	{
		i = skip_all_quotes(str, i);
		if (str[i] == '|')
			count++;
	}
	return (count);
}

t_instruct	init_tabinstruct(char *str)
{
	t_instruct	tab;
	int			i;
	int			j;

	tab.size = 0;
	if (!str)
		return (tab);
	tab.size = get_pipes_amt(str);
	tab.i_tab = malloc(sizeof(int) * (tab.size + 1));
	j = -1;
	i = -1;
	while (str[++i])
	{
		i = skip_all_quotes(str, i);
		if (str[i] == '|')
			tab.i_tab[++j] = PIPE;
	}
	tab.i_tab[++j] = 0;
	tab.dup_enter = -1;
	tab.dup_exit = -1;
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
	return (check_redirect(str, st));
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
	if (str[i] == '<' && str[i + 1] == '<')
		return (ft_putferror(ERR_PARSE, "<<", st, 1), 0);
	if (str[i] == '<')
		return (ft_putferror(ERR_PARSE, "<", st, 1), 0);
	if (str[i] == '>' && str[i + 1] == '>')
		return (ft_putferror(ERR_PARSE, ">>", st, 1), 0);
	if (str[i] == '>')
		return (ft_putferror(ERR_PARSE, ">", st, 1), 0);
	if ((str[i] == '\'' || str[i] == '"') && !opened)
		opened = str[i];
	return (check_string(str, i, opened, st));
}
