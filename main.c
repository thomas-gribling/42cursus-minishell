/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:46:51 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/25 09:07:12 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**g_envp;

static void	exe_command_quick(char *command)
{
	char	**paths;
	pid_t	p;

	paths = get_paths();
	command = try_path(paths, command);
	tab_free(paths);
	if (!command)
		return ;
	else
	{
		p = fork();
		if (p < 0)
			ft_putferror(ERR_CREATE, "fork", NULL, 0);
		if (p == 0)
			execve(command, &command, g_envp);
		if (p > 0)
			waitpid(p, NULL, 0);
	}
	free(command);
}

static void	print_header(void)
{
	printf(" ========================================\n");
	printf("            _       _     _          _ _\n");
	printf("           (_)     (_)   | |        | | |\n");
	printf("  _ __ ___  _ _ __  _ ___| |__   ___| | |\n");
	printf(" | '_ ` _ \\| | '_ \\| / __| '_ \\ / _ \\ | |\n");
	printf(" | | | | | | | | | | \\__ \\ | | |  __/ | |\n");
	printf(" |_| |_| |_|_|_| |_|_|___/_| |_|\\___|_|_|\n");
	printf("             by minishlags\n");
	printf(" ========================================\n\n");
}

static void	signal_ctrlc(int sigid)
{
	(void)sigid;
	rl_replace_line("", 0);
	rl_on_new_line();
	printf("\n");
	rl_redisplay();
}

static void	signal_init(void)
{
	struct sigaction	ctrl_c;

	signal(SIGINT, SIG_IGN);
	sigemptyset(&ctrl_c.sa_mask);
	ctrl_c.sa_handler = signal_ctrlc;
	ctrl_c.sa_flags = 0;
	sigaction(SIGINT, &ctrl_c, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int ac, char **av, char **envp)
{
	int			st;
	char		*line;
	t_instruct	instruct;

	(void)ac;
	(void)av;
	signal_init();
	g_envp = tab_dup(envp, 0);
	st = 0;
	exe_command_quick("clear");
	print_header();
	line = readline(PROMPT);
	while (line && ft_strncmp(line, "exit", 4))
	{
		if (verif_instruct(line, &st) && ft_strcmp(line, ""))
		{
			instruct = init_tabinstruct(line);
			start_parsing(line, &instruct, &st);
			add_history(line);
			if (!ft_strcmp(line, "clear"))
				print_header();
			free_instruct(&instruct);
		}
		free(line);
		line = readline(PROMPT);
	}
	printf("\n");
	free(line);
	tab_free(g_envp);
}
