/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:46:51 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/04 15:21:54 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**g_envp;

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

static void	init_all(int ac, char **av, char **envp, int *st)
{
	(void)ac;
	(void)av;
	signal_init();
	g_envp = tab_dup(envp, 0);
	*st = 0;
	exe_command_quick("clear");
	print_header();
}

int	main(int ac, char **av, char **envp)
{
	int			st;
	char		*line;
	t_instruct	instruct;

	init_all(ac, av, envp, &st);
	line = readline(PROMPT);
	while (line && ft_strncmp(line, "exit", 4))
	{
		if (ft_strcmp(line, "") && verif_instruct(line, &st))
		{
			add_history(line);
			instruct = init_tabinstruct(line);
			start_parsing(ft_strdup(line), &instruct, &st);
			if (!ft_strcmp(line, "clear"))
				print_header();
			free_instruct(&instruct);
		}
		free(line);
		line = readline(PROMPT);
	}
	if (!line || ft_strncmp(line, "exit", 4))
		printf("\n");
	free(line);
	tab_free(g_envp);
}
