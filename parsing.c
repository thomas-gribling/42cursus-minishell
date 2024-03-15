/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:36:14 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/15 15:56:38 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_char(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
		return (0);
	return (1);
}

int	parse_buffer(char *buffer, char **envp, t_instruct *instruct, int *i)
{
	char	*tmp;
	int		start;
	int		opened;

	start = 0;
	while (buffer[++(*i)])
	{
		if (buffer[*i] == '\'' || buffer[*i] == '"')
		{
			opened = buffer[(*i)];
			while (buffer[++(*i)] && buffer[*i] != opened)
				continue ;
		}
		if (!is_valid_char(buffer[*i]))
		{
			instruct->ind++;
			tmp = ft_substr(buffer, start, *i - start);
			start = *i + 1;
			while (!is_valid_char(buffer[++(*i)]) || buffer[*i] == ' ')
				start++;
			exe_command(tmp, envp, instruct);
			free(tmp);
		}
	}
	return (start);
}

void	start_parsing(char *buffer, char **envp, t_instruct *instruct)
{
	int		i;
	int		start;
	char	*tmp;

	i = -1;
	instruct->ind = -1;
	start = parse_buffer(buffer, envp, instruct, &i);
	if (start != i)
	{
		instruct->ind++;
		tmp = ft_substr(buffer, start, i);
		exe_command(tmp, envp, instruct);
		free(tmp);
	}
}
