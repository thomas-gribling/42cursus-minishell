/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:33:34 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/03 15:56:33 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_strjoin(char *s1, char *s2, int do_free)
{
	char	*s3;
	int		i;
	int		j;

	s3 = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (s3 == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		s3[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		s3[i + j] = s2[j];
		j++;
	}
	s3[i + j] = '\0';
	if (do_free == 1 || do_free == 3)
		free(s1);
	if (do_free == 2 || do_free == 3)
		free(s2);
	return (s3);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = -1;
	while (str[++i])
		write(fd, &str[i], 1);
}

void	ft_puterror(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
		write(2, &s[i], 1);
}

void	ft_putferror(char *s, char *arg, int *st, int new_st)
{
	int	i;

	i = -1;
	while (s[++i])
	{
		if (s[i] == '%' && s[i + 1] == 's')
		{
			ft_puterror(arg);
			i++;
		}
		else
			write(2, &s[i], 1);
	}
	if (st)
		*st = new_st;
}

void	tab_free(char **strs)
{
	int	i;

	i = -1;
	while (strs[++i])
		free(strs[i]);
	free(strs);
}
