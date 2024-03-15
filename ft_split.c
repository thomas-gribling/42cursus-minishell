/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 12:19:28 by ccadoret          #+#    #+#             */
/*   Updated: 2024/03/15 12:41:14 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	size_tab(char *s, int c)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0' && s[i] != c)
		{
			size++;
			while (s[i] != '\0' && s[i] != c)
				i++;
		}
	}
	return (size);
}

int	strstrlen(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i])
		i++;
	return (i);
}

char	*fill_tab(char *s, char c, char **tab, int i)
{
	int	size_low;	
	int	j;

	while (*s == c)
		s++;
	size_low = strstrlen(s, c);
	tab[i] = (char *)malloc(sizeof(char) * (size_low + 1));
	j = -1;
	while (++j < size_low)
	{
		tab[i][j] = *s;
		s++;
	}
	tab[i][j] = '\0';
	return (s);
}

char	**ft_split(char *s, char c)
{
	int		i;
	int		size;
	char	**tab;

	i = -1;
	if (s == NULL || !s)
		return (NULL);
	size = size_tab(s, c);
	tab = (char **)malloc(sizeof(char *) * (size + 1));
	if (tab == NULL)
		return (NULL);
	while (++i < size)
		s = fill_tab(s, c, tab, i);
	tab[i] = NULL;
	return (tab);
}
/*#include <stdio.h>
int	main(int argc, char **argv)
{
	int	i;
	char	**tab;
	//char	**tab2;

	if (argc != 3)
		return (1);
	i = -1;
	tab = ft_split(argv[1], argv[2][0]);
	if (!tab)
	{
		printf("vide");
		return (0);
	}
	while (tab[++i])
		printf("|%s|\n", tab[i]);
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
	tab2 = ft_split("aa\0bbb", '\0');
	if (!tab || tab == NULL)
		printf("vide");
	while (tab2[++i])
		printf("2:|%s|\n", tab2[i]);
	return (0);
}*/