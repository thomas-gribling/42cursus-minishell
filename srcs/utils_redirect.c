/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 08:38:32 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/04 11:05:59 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/gnl.h"

int	typeof_redirect(char *cmd, int i)
{
	if (cmd[i] == '<' && cmd[i + 1] == '<')
		return (HEREDOC_I);
	if (cmd[i] == '>' && cmd[i + 1] == '>')
		return (HEREDOC_O);
	if (cmd[i] == '<')
		return (REDIRECT_I);
	if (cmd[i] == '>')
		return (REDIRECT_O);
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
		if (append == HEREDOC_O)
			return (open(path, O_WRONLY | O_CREAT | O_APPEND, 0664));
		else
			return (open(path, O_WRONLY | O_CREAT | O_TRUNC, 0664));
	}
	return (0);
}

static void	read_stdin_loop(int *stdin_pipe, char *keyword, int pipes_amt)
{
	char	*buffer;
	char	*temp;
	char	*prompt;

	prompt = get_pipes_heredoc(pipes_amt);
	buffer = ft_strdup("");
	ft_putstr_fd(prompt, 1);
	temp = get_next_line(0);
	while (ft_strcmp(temp, keyword))
	{
		buffer = ft_strjoin(buffer, temp, 3);
		ft_putstr_fd(prompt, 1);
		temp = get_next_line(0);
	}
	ft_putstr_fd(buffer, stdin_pipe[1]);
	free(temp);
	free(buffer);
	free(prompt);
	exit(0);
}

void	read_stdin(int *stdin_pipe, char *keyword, int pipes_amt)
{
	pid_t	p;

	keyword = ft_strjoin(keyword, "\n", 0);
	p = fork();
	if (p < 0)
		return ;
	if (p == 0)
		read_stdin_loop(stdin_pipe, keyword, pipes_amt);
	if (p > 0)
		wait(&p);
	free(keyword);
}

int	is_it_redirect(char *s, int *i)
{
	char	c1;
	char	c2;

	c1 = s[*i + 1];
	if (!c1)
		return (0);
	c2 = s[*i + 2];
	if ((s[*i] == '>' && c1 == '>') || (s[*i] == '<' && c1 == '<'))
		if (c2 == ' ' || c2 == '\'' || c2 == '"')
			if (c2 == s[*i - 1])
				return ((*i)++, 1);
	if (s[*i] == '>' || s[*i] == '<')
		if (c1 == ' ' || c1 == '\'' || c1 == '"')
			if (c1 == s[*i - 1])
				return (1);
	return (0);
}
