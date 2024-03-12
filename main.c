/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:46:51 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/12 15:24:56 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_header(void)
{
	printf("            _       _     _          _ _\n");
	printf("           (_)     (_)   | |        | | |\n");
	printf("  _ __ ___  _ _ __  _ ___| |__   ___| | |\n");
	printf(" | '_ ` _ \\| | '_ \\| / __| '_ \\ / _ \\ | |\n");
	printf(" | | | | | | | | | | \\__ \\ | | |  __/ | |\n");
	printf(" |_| |_| |_|_|_| |_|_|___/_| |_|\\___|_|_|\n");
	printf("             by minishlags\n\n");
}

void	ctrlc_used(int sigid)
{
	(void)sigid;
	rl_replace_line("", 0);
	rl_on_new_line();
	printf("\n");
	rl_redisplay();
}

int	main(int ac, char **av, char **envp)
{
	char				*line;
	struct sigaction	ctrl_c;
	t_instruct			instruct;
	
	(void)ac;
	(void)av;
	ctrl_c.sa_handler = ctrlc_used;
	ctrl_c.sa_flags = 0;
	sigemptyset(&ctrl_c.sa_mask);
	signal(SIGINT, SIG_IGN);
	sigaction(SIGINT, &ctrl_c, NULL);
	signal(SIGQUIT, SIG_IGN);
	exe_command("clear", envp);
	print_header();
	line = readline("minishell$ ");
	while (line && ft_strcmp(line, "exit"))
	{
		instruct = init_tabinstruct(line);
		parse_buffer(line, envp, instruct);
		add_history(line);
		if (!ft_strcmp(line, "clear"))
			print_header();
		free(line);
		line = readline("minishell$ ");
	}
	free(line);
}
