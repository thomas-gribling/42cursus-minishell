/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 12:19:28 by ccadoret          #+#    #+#             */
/*   Updated: 2024/04/04 15:12:26 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	size_tab(char *s, int c, char t)
{
	int		i;
	int		size;

	i = 0;
	size = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != '\0' && s[i] != c)
		{
			size++;
			while (s[i] != '\0' && s[i] != c)
			{
				if (s[i] == '\'' || s[i] == '"')
				{
					t = s[i];
					while (s[++i] != t && s[i] != '\0')
						continue ;
				}
				i++;
			}
		}
	}
	return (size);
}

int	strstrlen(char *s, char c)
{
	int		i;
	int		nb;
	char	t;

	i = 0;
	nb = 0;
	while (s[i] != c && s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			t = s[i];
			nb++;
			while (s[++i] != t)
				continue ;
		}
		i++;
	}
	return (i - (nb * 2));
}

char	*in_cotes(char *s, char **tab, int i, int *j)
{
	char	t;

	t = '"';
	if (*s == '\'' || *s == '"')
	{
		t = *s;
		s++;
		while (*s != t && *s != '\0')
		{
			tab[i][*j] = *s;
			*j += 1;
			s++;
		}
		s++;
	}
	return (s);
}

char	*fill_tab(char *s, char c, char **tab, int i)
{
	int		size_low;	
	int		j;

	while (*s == c)
		s++;
	size_low = strstrlen(s, c);
	tab[i] = (char *)malloc(sizeof(char) * (size_low + 2));
	j = -1;
	while (++j < size_low)
	{
		s = in_cotes(s, tab, i, &j);
		if (*s == c)
		{
			tab[i][j] = '\0';
			return (s);
		}
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
	size = size_tab(s, c, '"');
	tab = (char **)malloc(sizeof(char *) * (size + 1));
	if (tab == NULL)
		return (NULL);
	while (++i < size)
		s = fill_tab(s, c, tab, i);
	tab[i] = NULL;
	return (tab);
}

/*int	main(int argc, char **argv)
{
	int	i;
	char	**tab;

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
}*/