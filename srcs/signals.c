/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 10:11:28 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/05 10:13:02 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ctrl_c_parent(int sigid)
{
	(void)sigid;
	rl_replace_line("", 0);
	rl_on_new_line();
	printf("\n");
	rl_redisplay();
}

static void	ctrl_c_child(int sigid)
{
	(void)sigid;
	printf("\n");
}

static void	init_ctrl_c(int type)
{
	struct sigaction	ctrl_c;

	sigemptyset(&ctrl_c.sa_mask);
	if (type == 0)
		ctrl_c.sa_handler = ctrl_c_parent;
	else
		ctrl_c.sa_handler = ctrl_c_child;
	ctrl_c.sa_flags = 0;
	sigaction(SIGINT, &ctrl_c, NULL);
}

void	signal_init(int type)
{
	signal(SIGINT, SIG_IGN);
	init_ctrl_c(type);
	signal(SIGQUIT, SIG_IGN);
}
