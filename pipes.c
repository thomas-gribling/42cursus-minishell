/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:41:56 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/02 16:42:08 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_pipes(t_instruct *ins, int close_before, int close_curr)
{
	int	i;

	i = -1;
	while (++i < ins->size)
	{
		if (i == ins->ind && !close_curr)
			continue ;
		else if (i == ins->ind - 1 && !close_before)
			continue ;
		else
		{
			close(ins->pipes[i][0]);
			close(ins->pipes[i][1]);
		}
	}
}

static void	dup_redirects(t_instruct *ins)
{
	close(ins->pipe_heredoc[1]);
	if (ins->dup_enter != ins->pipe_heredoc[0])
		close(ins->pipe_heredoc[0]);
	if (ins->dup_enter != -1)
		dup2(ins->dup_enter, 0);
	if (ins->dup_exit != -1)
		dup2(ins->dup_exit, 1);
}

void	dup_fds(t_instruct *ins, int do_pipe)
{
	dup_redirects(ins);
	if (do_pipe == 0)
		close_all_pipes(ins, 1, 0);
	if (do_pipe == 1)
	{
		dup2(ins->pipes[ins->ind - 1][0], 0);
		close(ins->pipes[ins->ind - 1][1]);
		close_all_pipes(ins, 0, 1);
	}
	if (do_pipe == 2)
	{
		dup2(ins->pipes[ins->ind][1], 1);
		close(ins->pipes[ins->ind][0]);
		close_all_pipes(ins, 1, 0);
	}
	if (do_pipe == 3)
	{
		dup2(ins->pipes[ins->ind - 1][0], 0);
		close(ins->pipes[ins->ind - 1][1]);
		dup2(ins->pipes[ins->ind][1], 1);
		close(ins->pipes[ins->ind][0]);
		close_all_pipes(ins, 0, 0);
	}
}
