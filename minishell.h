/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:43:11 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/29 01:00:04 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <dirent.h>
# include <termios.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

//Token Structure Nodes
typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SINGLE_QUOTED,
	TOKEN_DOUBLE_QUOTED,
	TOKEN_VARIABLE
}	t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	int					has_space_before;
	struct s_token		*next;
}	t_token;

//AST Structure Nodes
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE
}	t_node_type;

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast
{
	t_node_type		type;
	char			**argv;
	t_redir			*redirs;
	int				is_builtin;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

//Env vars List
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

//General Info Structure
typedef struct s_info
{
	char	*line;
	t_env	*env_list;
	char	**env_array;
	int		last_status;
	int		had_space;
	t_token	*tokens;
	int		pipe_count;
	t_ast	*tree;
	t_ast	**cmds;
	char	*heredoc_filename;
	int		last_pipe_read_fd;
	int		last_pipe_write_fd;
	int		leftover_read_fd;
	pid_t	*child_pids;
	int		child_count;
}	t_info;

//Singleton struct
typedef struct s_terminal
{
	t_info	info;
}	t_terminal;

//global
extern volatile sig_atomic_t	g_last_signal;

//cleaner
void			clean_shell(t_info *info);
void			clean_loop(t_info *info);
void			free_tokens(t_token *token);
void			free_heredoc(char **filename);
void			free_ast(t_ast *node);
t_env			*free_env(t_env *env);
void			free_redirs(t_redir *redir);
void			free_array(char **arr);
void			cleanup_heredoc_files(void);
void			close_parent_fds(t_info *info);
void			kill_all_children(t_info *info);
void			reap_children(t_info *info, int i);

//error
void			child_exit(char *message, int code, t_info *info, char *file);
void			parent_exit(char *message, t_info *info);
void			parent_return(char *message, t_info *info, int status, char *f);
void			exit_exec_error(const char *cmd, t_info *info, char *path);

//env
void			env_list_to_array(t_info *info);
void			populate_env(char **envp, t_info *info);
t_env			*env_init(char **envp);

//env_ops
char			*get_env_value(t_env *env_list, const char *key);
void			set_env_value(t_env **env_list, const char *key,
					const char *value);
void			unset_env(t_env **env_list, const char *key);

//lexing
int				ft_isspace(char c);
int				is_operator_redir_char(char c);
void			malloc_error_lexing(t_token **tokens, char **buf, t_info *info);
void			skip_spaces_and_mark(const char *line, int *i, t_info *info);
void			handle_variable(t_info *info, int *i, char **buf,
					t_token **tokens);
int				append_char(char **buf, char c);
int				append_token(t_token **tokens, char **buf, t_info *info);
t_token			*new_token(char *value);
int				add_token(t_token **head, t_token *new, t_info *info);
void			classify_tokens(t_token *tokens);
int				ft_is_valid(char *name, int i, char delim);
int				is_pipe(t_token *token);
int				is_redirect(t_token *token);
t_token			*token_error(t_token **tokens);
void			error_tokens(t_token **tokens, const char *value);
t_token			*finalize_tokens(t_info *info, t_token **tokens);
t_token			*lexing(t_info *info);

//expansion
int				join_non_operator_tokens(t_token **tokens);
int				remove_empty_tokens_from_list(t_token **tokens);
char			*expand_inside_quotes(t_info *info, const char *str);
char			*expand_var_in_quotes(t_info *info, const char *str, int *i);
char			*append_piece(t_info *info, const char *str, int *i, char *res);
char			*expand_var_value(t_info *info, const char *key);
void			expand_variables(t_info *info);

//parsing
int				is_builtin(char *str);
t_redir_type	redir_type(t_token *redir);
t_token			*find_pipe(t_token *tokens, int n);
void			parsing(t_info *info, int pipe_count);

//path
char			*get_path(t_info *info, t_ast *cmd);

//command execution
void			executor(t_ast *node, t_info *info);
t_ast			**flatten_pipeline(t_ast *root, int *out_count);
void			exec_pipeline(t_ast **cmds, int count,
					t_info *info, int input_fd);
void			exec_command(t_ast *cmd, t_info *info, int root);

//redirections
void			handle_redirections(t_redir *redir, t_info *info);
int				child_heredocs(t_redir *redir, int *j, t_info *info);
int				prepare_heredocs(t_ast **cmds, t_info *info, int count);

//builtins
void			builtin_echo(t_ast *ast, int root, t_info *info);
int   			update_env(t_info *info, char *path, int root);
void			builtin_cd(t_ast *ast, t_info *info, int root);
void			builtin_pwd(t_info *info, int root);
void			builtin_export(int root, t_ast *cmd, t_info *info);
void			builtin_unset(int root, t_ast *ast, t_info *info);
void			builtin_env(int root, t_ast *ast, t_info *info);
void			builtin_exit(t_ast *ast, t_info *info, int root);

//signals
void			prompt_sigint_handler(int sig);
void			child_sigint_handler(int sig);
void			child_sigquit_handler(int sig);

#endif