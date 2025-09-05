/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:20:32 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/05 00:59:32 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

volatile sig_atomic_t	g_last_signal;

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_shell	shell;
	
	shell.envp = envp;
	shell.last_status = 0;
	/*while (1)
	{
		char *line = readline("minishell$ ");
		t_ast *ast = parse(line);
		executor(ast, shell.envp); // pass env explicitly
	}*/
	t_command cmd_ls;
	cmd_ls.argv = (char *[]){"cat", "hello.txt", NULL};
	cmd_ls.redirs = NULL;

	// Build AST node
	t_ast node_ls;
	node_ls.type = NODE_COMMAND;
	node_ls.u_data.cmd = &cmd_ls;

	// Execute
	shell.last_status = exec_command(node_ls.u_data.cmd, &shell);
	printf("Exit status: %d\n", shell.last_status);
	return (0);
}
