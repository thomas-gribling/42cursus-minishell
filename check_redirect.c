/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 13:20:43 by ccadoret          #+#    #+#             */
/*   Updated: 2024/04/02 13:20:43 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirect(char *str)
{
	int		i;
	int		last;
	char	**strs;

	i = -1;
	last = -1;
	strs = ft_split(str, ' ');
	while (strs[++i])
	{
		if (typeof_redirect(strs[i]))
		{
			if (typeof_redirect(strs[i - 1]))
				return (free(strs), 0);
			if (typeof_redirect(strs[i + 1]))
				return (free(strs), 0);
			if (typeof_redirect(strs[i]) == last)
				return (free(strs), 0);
			last = typeof_redirect(strs[i]);
		}
		if (ft_strcmp(strs[i], "|"))
			last = 0;
	}
	return (free(strs), 1);
}
