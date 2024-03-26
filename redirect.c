/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:31:23 by ccadoret          #+#    #+#             */
/*   Updated: 2024/03/26 17:12:51 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	typeof_redirect(char *cmd)
{
	if (!ft_strcmp(cmd, "<"))
		return (REDIRECT_I);
	if (!ft_strcmp(cmd, "<<"))
		return (HEREDOC_I);
	if (!ft_strcmp(cmd, ">"))
		return (REDIRECT_O);
	if (!ft_strcmp(cmd, ">>"))
		return (HEREDOC_O);
	return (0);
}

void	do_redirects(t_instruct *ins, int *st, t_redirect *red)
{
	int	i;
	int	j;

	i = -1;
	(void)ins;
	(void)st;
	while (red->cmds[++i])
	{
		j = -1;
		while (red->cmds[i][++j])
			printf("[%s]", red->cmds[i][j]);
		printf(" - %d\n", red->redirs[i]);
	}
}

void	fill_redirect(t_instruct *ins, char **cmd, int *st, t_redirect *red)
{
	int	i;
	int	j;
	int	start;

	red->cmds = malloc((red->count + 2) * sizeof(char **));
	red->redirs = malloc((red->count + 2) * sizeof(int));
	i = -1;
	j = -1;
	start = 0;
	while (cmd[++i])
	{
		if (!cmd[i + 1] || typeof_redirect(cmd[i]))
		{
			red->redirs[++j] = typeof_redirect(cmd[i]);
			if (i > 0)
				red->cmds[j] = tab_dup_n(cmd, start, i - start + !cmd[i + 1]);
			else
			{
				red->cmds[j] = malloc(sizeof(char *));
				red->cmds[j][0] = NULL;
			}
			start = i + 1;
		}
	}
	do_redirects(ins, st, red);
}

int	is_redirect(t_instruct *ins, char *command, int *st)
{
	char		**cmd;
	t_redirect	red;
	int			i;
	int			opened;

	opened = 0;
	i = -1;
	while (command[++i])
	{
		if (command[i] == '\'' || command[i] == '"')
		{
			if (!opened)
				opened = command[i];
			else if (opened == command[i])
				opened = 0;
		}
		if (!opened && (command[i] == '>' || command[i] == '<'))
			break ;
	}
	if (!command[i])
		return (0);
	cmd = ft_split(command, ' ');
	i = -1;
	red.count = 0;
	while (cmd[++i])
		if (typeof_redirect(cmd[i]))
			red.count++;
	fill_redirect(ins, cmd, st, &red);
	return (1);
}
