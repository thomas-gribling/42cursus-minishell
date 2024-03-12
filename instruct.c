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
	tab.tab_instruct = malloc(sizeof(int) * tab.size);
	j = -1;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '|')
			tab.tab_instruct[++j] = PIPE;
	}
	return (tab);
}
