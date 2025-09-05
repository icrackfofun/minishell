/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:43:11 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/05 00:49:26 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "libft/libft.h"
# include <unistd.h>   // execve, fork
# include <sys/wait.h> // waitpid
# include <stdio.h>    // perror
# include <stdlib.h>   // exit

//shell information structure
typedef struct s_shell
{
	char	**envp; // environment variables
	int		last_status; // last command exit status
}	t_shell;

//AST Structure Nodes
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE
}	t_node_type;

typedef struct s_redir
{
	int				type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char	**argv;
	t_redir	*redirs;
	int		is_builtin;
}	t_command;

typedef struct s_pipe
{
	struct s_ast	*left;
	struct s_ast	*right;
}	t_pipe;

typedef struct s_ast
{
	t_node_type	type;
	union
	{
		t_pipe		*pipe;
		t_command	*cmd;
	}	u_data;
}	t_ast;

//command
int		exec_command(t_command *cmd, t_shell *shell);

//path
char	*get_path(t_command *cmd);

#endif