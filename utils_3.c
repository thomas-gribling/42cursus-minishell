/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:06:39 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/19 08:32:43 by tgriblin         ###   ########.fr       */
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

char	**tab_dup(char **tab, int start)
{
	char	**out;
	int		i;

	i = 0;
	while (tab[start + i])
		i++;
	out = malloc((i + 1) * sizeof(char *));
	i = start - 1;
	while (tab[++i])
		out[i - start] = ft_strdup(tab[i]);
	out[i - start] = NULL;
	return (out);
}
