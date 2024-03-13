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
			ft_putstr_fd("pipe error\n", 2);
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
	tab.i_tab = malloc(sizeof(int) * tab.size);
	j = -1;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '|')
			tab.i_tab[++j] = PIPE;
	}
	pipes_init(&tab);
	tab.envp = envp;
	return (tab);
}
