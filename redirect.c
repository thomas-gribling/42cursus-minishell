/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:31:23 by ccadoret          #+#    #+#             */
/*   Updated: 2024/04/02 10:53:49 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "gnl.h"

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

int	try_open(char *path, int *st, int input, int append)
{
	if (input)
	{
		if (access(path, F_OK))
			return (ft_putferror(ERR_NOFILE, path, st, 1), 0);
		if (access(path, R_OK))
			return (ft_putferror(ERR_NOPERM, path, st, 1), 0);
		return (open(path, O_RDONLY));
	}
	else
	{
		if (!access(path, F_OK) && access(path, W_OK))
			return (ft_putferror(ERR_NOPERM, path, st, 1), 0);
		if (append)
			return (open(path, O_WRONLY | O_CREAT | O_APPEND, 0664));
		else
			return (open(path, O_WRONLY | O_CREAT | O_TRUNC, 0664));
	}
	return (0);
}

void	read_stdin(int *stdin_pipe, char *keyword)
{
	pid_t	p;
	char	*buffer;
	char	*temp;

	keyword = ft_strjoin(keyword, "\n", 0);
	p = fork();
	if (p < 0)
		return ;
	if (p == 0)
	{
		buffer = ft_strdup("");
		ft_putstr_fd("heredoc> ", 1);
		temp = get_next_line(0);
		while (ft_strcmp(temp, keyword))
		{
			buffer = ft_strjoin(buffer, temp, 3);
			ft_putstr_fd("heredoc> ", 1);
			temp = get_next_line(0);
		}
		ft_putstr_fd(buffer, stdin_pipe[1]);
		free(temp);
		free(buffer);
		exit(0);
	}
	if (p > 0)
		wait(&p);
	free(keyword);
}

void	do_redirects(t_instruct *ins, int *st, t_redirect *red)
{
	int	i;

	i = -1;
	pipe(ins->pipe_heredoc);
	ins->dup_enter = 0;
	ins->dup_exit = 1;
	while (red->cmds[++i])
	{
		if (red->redirs[i] == REDIRECT_I)
		{
			ins->dup_enter = try_open(red->cmds[i + 1][0], st, 1, 0);
			if (!ins->dup_enter)
				return ;
		}
		else if (red->redirs[i] == HEREDOC_I)
		{
			read_stdin(ins->pipe_heredoc, red->cmds[i + 1][0]);
			ins->dup_enter = ins->pipe_heredoc[0];
		}
		else if (red->redirs[i])
		{
			ins->dup_exit = try_open(red->cmds[i + 1][0], st, 0, red->redirs[i] == HEREDOC_O);
			if (!ins->dup_exit)
				return ;
		}
	}
	exe_command(red->cmds[0], ins, st);
	if (!ins->dup_enter)
		close(ins->pipe_heredoc[0]);
	close(ins->pipe_heredoc[1]);
}

void	fill_redirect(t_instruct *ins, char **cmd, int *st, t_redirect *red)
{
	int	i;
	int	j;
	int	start;

	red->cmds = malloc((red->count + 2) * sizeof(char **));
	red->redirs = malloc((red->count + 1) * sizeof(int));
	i = -1;
	j = -1;
	start = 0;
	while (cmd[++i])
	{
		if (!cmd[i + 1] || typeof_redirect(cmd[i]))
		{
			red->redirs[++j] = typeof_redirect(cmd[i]);
			if (i > 0)
			{
				if (!cmd[i + 1] && !typeof_redirect(cmd[i]))
					red->cmds[j] = tab_dup_n(cmd, start, i - start + 1);
				else
					red->cmds[j] = tab_dup_n(cmd, start, i - start);
			}
			else
			{
				red->cmds[j] = malloc(sizeof(char *));
				red->cmds[j][0] = NULL;
			}
			start = i + 1;
		}
	}
	red->cmds[++j] = NULL;
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
