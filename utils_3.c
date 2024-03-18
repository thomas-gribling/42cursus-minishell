/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:06:39 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/18 16:47:46 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_append(char *old, char c)
{
	int		i;
	char	*new;

	new = malloc(ft_strlen(old) + 2);
	i = -1;
	while (old && old[++i])
		new[i] = old[i];
	if (i == -1)
		i++;
	new[i] = c;
	new[++i] = '\0';
	free(old);
	return (new);
}
