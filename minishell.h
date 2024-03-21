/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccadoret <ccadoret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:08:28 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/21 16:27:05 by ccadoret         ###   ########.fr       */
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

# define LIME "\033[0;92m"
# define SKY_BLUE "\033[0;96m"
# define RESET "\033[0m"
# define WHITE "\033[0;37m"
# define PROMPT LIME "➜  " SKY_BLUE "minishell" RESET " "

# define ERR_NOCMD "%s: command not found\n"
# define ERR_NOFILE "%s: no such file or directory\n"
# define ERR_NOPERM "%s: permission denied\n"
# define ERR_NODIRPERM "%s: not a directory or permission denied\n"
# define ERR_CREATE	 "%s: unable to create %s\n"
# define ERR_NEARGS "%s: not enough arguments\n"
# define ERR_TMARGS "%s: too many arguments\n"
# define ERR_PARSE "minishell: parse error near `%s'\n"
# define ERR_UNCLOSED "minishell: unclosed %s detected\n"

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

extern char	**g_envp;

void		start_parsing(char *buffer, t_instruct *instruct);

void		exe_command(char *command, t_instruct *ins);
int			exe_builtin(t_instruct *ins, char **cmd);
void		dup_fds(t_instruct *ins, int do_pipe);
void		close_all_pipes(t_instruct *ins, int close_before, int close_curr);

int			is_valid_char(char c);

void		update_pwd(void);
void		ft_export(char *new_var);
int			find_var(char *var_name);
void		ft_unset_export(char **cmd);
char		*ft_getenv(char *var);
void		ft_unset(char *cmd);

char		**get_paths(void);
char		*try_path(char **strs, char *str);

void		free_instruct(t_instruct *tab);
t_instruct	init_tabinstruct(char *str);
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