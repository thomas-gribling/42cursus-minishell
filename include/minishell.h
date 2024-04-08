/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:08:28 by tgriblin          #+#    #+#             */
/*   Updated: 2024/04/05 11:44:53 by tgriblin         ###   ########.fr       */
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

# define PROMPT "\033[0;92m➜  \033[0;96mminishell \033[0m"

# define ERR_NOCMD "%s: command not found\n"
# define ERR_NOFILE "%s: no such file or directory\n"
# define ERR_NOPERM "%s: permission denied\n"
# define ERR_NODIRPERM "%s: not a directory or permission denied\n"
# define ERR_CREATE	 "%s: unable to create %s\n"
# define ERR_NEARGS "%s: not enough arguments\n"
# define ERR_TMARGS "%s: too many arguments\n"
# define ERR_PARSE "minishell: parse error near `%s'\n"
# define ERR_UNCLOSED "minishell: unclosed %s detected\n"
# define ERR_VARNAME "%s: invalid variable name\n"
# define ERR_VARMISS "%s: missing variable name\n"

typedef struct s_instruct
{
	pid_t	p;
	int		size;
	int		ind;
	int		dup_enter;
	int		dup_exit;
	int		do_wait;
	int		*i_tab;
	int		pipe_heredoc[2];
	int		*var_tab;
	int		**pipes;
	char	**envp;
}			t_instruct;

typedef struct s_redirect
{
	char	***cmds;
	int		*redirs;
	int		*validity;
	int		cv;
	int		count;
	int		i;
	int		j;
	int		k;
	int		tmp;
}			t_redirect;

extern char	**g_envp;

void		signal_init(int type);

void		start_parsing(char *buffer, t_instruct *instruct, int *st);
int			skip_all_quotes(char *str, int i);

char		*replace_vars(t_instruct *ins, char *old, int *st);
char		*replace_roots(char *old);

int			exe_command(char **cmd, t_instruct *ins, int *st);
void		exe_command_quick(char *command, char **paths, char *cmd);
int			is_redirect(t_instruct *ins, char *command, int *st);
int			exe_builtin(t_instruct *ins, char **cmd, int *st);
void		dup_fds(t_instruct *ins, int do_pipe);
void		close_all_pipes(t_instruct *ins, int close_before, int close_curr);

int			check_redirect(char *str, int *st);
void		fill_redir(t_instruct *ins, char *buff, int *st, t_redirect *red);

int			is_valid_char(char c);

void		call_builtin(t_instruct *ins);
void		ft_pwd_env(char **cmd, t_instruct *ins, int *st);
void		ft_echo(char **cmd, t_instruct *ins);
void		ft_cd(char **cmd, int *st);
void		update_pwd(void);
void		ft_export(char *new_var);
int			find_var(char *var_name);
void		ft_unset_export(char **cmd, int *st, t_instruct *ins);
char		*ft_getenv(char *var);
void		ft_varcpy(int size_path, int ind, char *var);
void		ft_unset(char *cmd, int *st);

char		**get_paths(void);
char		*try_path(char **strs, char *str);

void		free_instruct(t_instruct *tab);
t_instruct	init_tabinstruct(char *str);
int			verif_instruct(char *str, int *st);
void		vars_init(t_instruct *instruct, char *str);
void		pipes_init(t_instruct *instruct);

int			try_open(char *path, int *st, int input, int append);
void		read_stdin(int *stdin_pipe, char *keyword, int pipes_amt);
int			is_it_redirect(char *s, int *i);
int			typeof_redirect(char *cmd, int i);

// Utils
int			ft_strlen(char *s);
int			ft_strcmp(char *s1, char *s2);
int			ft_strncmp(char *s1, char *s2, unsigned int n);
char		*ft_strdup(char *src);
char		*ft_substr(char *s, int start, int len);
char		*ft_strjoin(char *s1, char *s2, int do_free);
void		ft_putstr_fd(char *str, int fd);
void		ft_puterror(char *s);
void		ft_putferror(char *s, char *arg, int *st, int new_st);
void		tab_free(char **strs);
char		*str_append(char *old, char c);
char		**tab_dup(char **tab, int start);
char		**tab_dup_n(char **tab, int start, int n);
char		*get_pipes_heredoc(int pipes_amt);
int			is_empty_char(char *s);

char		**ft_split(char *s, char c);
char		*ft_itoa(int n);

#endif