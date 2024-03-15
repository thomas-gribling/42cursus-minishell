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
			ft_puterror("pipe: unable to create pipe\n");
	}
}

t_instruct	init_tabinstruct(char *str, char **envp)
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
	tab.envp = envp;
	return (tab);
}

void	free_instruct(t_instruct *tab)
{
	int	i;

	free(tab->i_tab);
	close_all_pipes(tab, 1, 1);
	i = -1;
	while (++i < tab->size)
		free(tab->pipes[i]);
	free(tab->pipes);
}

int	check_string(char *str, int i, int opened)
{
	while (str[++i])
	{
		if (opened || str[i] == '\'' || str[i] == '"')
		{
			if (!opened)
				opened = str[i];
			while (str[++i] && str[i] != opened)
				continue ;
			if (str[i] == '\0')
				return (ft_puterror("minishell: unclosed quotes\n"), 0);
			opened = 0;
		}
		if (str[i] == '|' && !opened)
		{
			while (str[++i] == ' ')
				continue ;
			if (str[i] == '|' || !str[i])
				return (ft_puterror("minishell: parse error near `|'\n"), 0);
		}
		if (str[i] == '&')
			return (ft_puterror("minishell: parse error near `&'\n"), 0);
	}
	return (1);
}

int	verif_instruct(char *str)
{
	int	i;
	int	opened;

	i = -1;
	opened = 0;
	while (str[++i] == ' ')
		continue ;
	if (str[i] == '|')
		return (ft_puterror("minishell: parse error near `|'\n"), 0);
	if (str[i] == '&')
		return (ft_puterror("minishell: parse error near `&'\n"), 0);
	if (str[i] == '\'' || str[i] == '"')
		opened = str[i];
	return (check_string(str, i, opened));
}
