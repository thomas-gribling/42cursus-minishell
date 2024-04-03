/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:06:39 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/03 15:56:43 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

char	**tab_dup_n(char **tab, int start, int n)
{
	char	**out;
	int		i;

	i = 0;
	while (tab[start + i] && i < n)
		i++;
	out = malloc((i + 1) * sizeof(char *));
	i = start - 1;
	while (tab[++i] && i - start < n)
		out[i - start] = ft_strdup(tab[i]);
	out[i - start] = NULL;
	return (out);
}

char	*get_pipes_heredoc(int pipes_amt)
{
	int		i;
	char	*out;

	out = ft_strdup("");
	i = -1;
	while (++i < pipes_amt)
		out = ft_strjoin(out, "pipe ", 1);
	out = ft_strjoin(out, "heredoc> ", 1);
	return (out);
}
