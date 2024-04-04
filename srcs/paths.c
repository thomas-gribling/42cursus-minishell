/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:29:15 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/04 14:15:50 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**get_paths(void)
{
	char	**strs;
	char	*tmp;

	tmp = ft_getenv("PATH");
	strs = ft_split(tmp, ':');
	free(tmp);
	return (strs);
}

static char	*join_path(char *path, char *file)
{
	char	*str;
	char	*str2;

	str = ft_strjoin(path, "/", 0);
	str2 = ft_strjoin(str, file, 1);
	return (str2);
}

char	*try_path(char **strs, char *str)
{
	int		i;
	char	*path;

	i = 0;
	while (strs[i])
	{
		path = join_path(strs[i], str);
		if (access(path, X_OK) == 0)
			return (free(str), path);
		free(path);
		i++;
	}
	free(str);
	return (NULL);
}
