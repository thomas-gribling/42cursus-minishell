/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:40:37 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/04 15:09:51 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	dup_loop(t_instruct *ins, t_redirect *r, int j, int *st)
{
	int	i;

	i = -1;
	while (++i < r->count)
	{
		if (!r->validity[i])
			continue ;
		else if (r->redirs[++j] == REDIRECT_I)
		{
			ins->dup_enter = try_open(r->cmds[j + 1][0], st, 1, 0);
			if (!ins->dup_enter)
				return ;
		}
		else if (r->redirs[j] == HEREDOC_I)
		{
			read_stdin(ins->pipe_heredoc, r->cmds[j + 1][0], ins->size);
			ins->dup_enter = ins->pipe_heredoc[0];
		}
		else if (r->redirs[j])
		{
			ins->dup_exit = try_open(r->cmds[j + 1][0], st, 0, r->redirs[j]);
			if (!ins->dup_exit)
				return ;
		}
	}
}

void	do_redirects(t_instruct *ins, int *st, t_redirect *red)
{
	int	j;

	j = -1;
	pipe(ins->pipe_heredoc);
	ins->dup_enter = 0;
	ins->dup_exit = 1;
	dup_loop(ins, red, j, st);
	exe_command(red->cmds[0], ins, st);
	if (!ins->dup_enter)
		close(ins->pipe_heredoc[0]);
	close(ins->pipe_heredoc[1]);
}

void	parse_loop(t_redirect *red, char *s, char *temp, int start)
{
	red->i = -1;
	red->j = -1;
	red->k = -1;
	while (s[++red->i])
	{
		red->tmp = typeof_redirect(s, red->i);
		if (!s[red->i + 1] || is_it_redirect(s, &red->i))
		{
			if (red->validity[++red->k])
			{
				red->redirs[++red->j] = red->tmp;
				if (red->tmp == HEREDOC_I || red->tmp == HEREDOC_O)
					red->i--;
				temp = ft_substr(s, start, red->i - start + !s[red->i + 1]);
				red->cmds[red->j] = ft_split(temp, ' ');
				free(temp);
				start = red->i + 1;
				if (red->tmp == HEREDOC_I || red->tmp == HEREDOC_O)
					start++;
			}
		}
	}
}

void	fill_redir(t_instruct *ins, char *buff, int *st, t_redirect *red)
{
	red->cmds = malloc((red->cv + 2) * sizeof(char **));
	red->redirs = malloc((red->cv + 1) * sizeof(int));
	parse_loop(red, buff, NULL, 0);
	red->cmds[++red->j] = NULL;
	do_redirects(ins, st, red);
}
