/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:08:28 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/19 14:12:35 by ccadoret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <dirent.h>
# include <sys/signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define SIGHUP  1   /* Hangup the process */
# define SIGINT  2   /* Interrupt the process */ // CTRL-C
# define SIGQUIT 3   /* Quit the process */ // CTRL-backslash
# define SIGILL  4   /* Illegal instruction. */
# define SIGTRAP 5   /* Trace trap. */
# define SIGABRT 6   /* Abort. */

# define PIPE 1
# define REDIRECT_I 2
# define REDIRECT_O 3
# define HEREDOC_I 4
# define HEREDOC_O 5

typedef struct s_instruct
{
	pid_t	p;
	int		size;
	int		ind;
	int		*i_tab;
	int		*var_tab;
	int		**pipes;
	char	**envp;
}			t_instruct;

void		start_parsing(char *buffer, char **envp, t_instruct *instruct);

void		exe_command(char *command, char **envp, t_instruct *ins);
int			exe_builtin(t_instruct *ins, char **cmd, char **envp);
void		dup_fds(t_instruct *ins, int do_pipe);
void		close_all_pipes(t_instruct *ins, int close_before, int close_curr);

char		*replace_root(char *old);
int			is_valid_char(char c);

char		*ft_getenv(char *var, char **envp);
void		ft_unset(char **cmd, char **envp);

char		**get_paths(char **envp);
char		*try_path(char **strs, char *str);

void		free_instruct(t_instruct *tab);
t_instruct	init_tabinstruct(char *str, char **envp);
int			verif_instruct(char *str);

// Utils
int			ft_strlen(char *s);
int			ft_strcmp(char *s1, char *s2);
int			ft_strncmp(char *s1, char *s2, unsigned int n);
char		*ft_strdup(char *src);
char		*ft_substr(char *s, int start, int len);
char		*ft_strjoin(char *s1, char *s2, int do_free);
void		ft_putstr_fd(char *str, int fd);
void		ft_puterror(char *s);
void		ft_putferror(char *s, char *arg);
void		tab_free(char **strs);
char		*str_append(char *old, char c);
char		**tab_dup(char **tab, int start);

char		**ft_split(char *s, char c);

#endif