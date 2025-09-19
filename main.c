/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:20:32 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/19 00:05:29 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_last_signal;

void print_tokens(t_token *tokens)
{
    t_token *tmp = tokens;

    while (tmp)
    {
        printf("Token: '%s' | Type: ", tmp->value);
        switch (tmp->type)
        {
            case TOKEN_WORD:           printf("WORD"); break;
            case TOKEN_PIPE:           printf("PIPE"); break;
            case TOKEN_REDIRECT_IN:    printf("REDIRECT_IN"); break;
            case TOKEN_REDIRECT_OUT:   printf("REDIRECT_OUT"); break;
            case TOKEN_REDIRECT_APPEND:printf("REDIRECT_APPEND"); break;
            case TOKEN_HEREDOC:        printf("HEREDOC"); break;
            case TOKEN_SINGLE_QUOTED:  printf("SINGLE_QUOTED"); break;
            case TOKEN_DOUBLE_QUOTED:  printf("DOUBLE_QUOTED"); break;
            case TOKEN_VARIABLE:       printf("VARIABLE"); break;
            default:                   printf("UNKNOWN");
        }
        printf("\n");
        tmp = tmp->next;
    }
}

static void	info_init(t_info *info, char **envp)
{
	info->env_list = env_init(envp);
	if (!info->env_list)
		parent_exit("malloc", info);
	info->env_array = NULL;
	info->last_status = 0;
	info->tree = NULL;
	info->tokens = NULL;
	info->child_pids = NULL;
	info->child_count = 0;
	info->cmds = NULL;
	info->last_pipe_read_fd = -1;
	info->last_pipe_write_fd = -1;
	info->leftover_read_fd = -1;
	g_last_signal = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	disable_ctrl_echo();
}

int	main(int argc, char **argv, char **envp)
{
	t_info	info;

	(void)argc;
	(void)argv;
	info_init(&info, envp);
	while (1)
	{
		info.line = readline("minishell$ ");
		if (!info.line)
		{
			printf("exit\n");
			break ;
		}
		if (*info.line)
			add_history(info.line);
		info.tokens = lexing(&info);
		print_tokens(info.tokens);
		printf("-----------------------------------\n");
		expand_variables(&info, info.tokens);
		if (info.tokens)
			print_tokens(info.tokens);
		//parse(&info);
		if (info.tree)
			executor(info.tree, &info);
		clean_loop(&info);
	}
	clean_shell(&info);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_info	info;
// 	(void)argc;
// 	(void)argv;
// 	info_init(&info, envp);
	
// 	// //pwd
// 	// t_ast	*node;
// 	// node = malloc(sizeof(t_ast));
// 	// node->type = NODE_COMMAND;
// 	// node->argv = malloc(3 * sizeof(char *));
// 	// node->argv[0] = ft_strdup("sleep");
// 	// node->argv[1] = ft_strdup("20");
// 	// node->argv[2] = NULL;
// 	// node->redirs = NULL;
// 	// node->is_builtin = 0;
// 	// node->left = NULL;
// 	// node->right = NULL;
// 	// info.tree = node;

// 	//echo -n Hello
// 	t_ast	*node;
// 	node = malloc(sizeof(t_ast));
// 	node->type = NODE_COMMAND;
// 	node->argv = malloc(3 * sizeof(char *));
// 	node->argv[0] = ft_strdup("echo");
// 	node->argv[1] = ft_strdup("hello world here i go");
// 	node->argv[2] = NULL;
// 	node->redirs = NULL;
// 	node->is_builtin = 0;
// 	node->left = NULL;
// 	node->right = NULL;
// 	info.tree = node;

// 	// //cd
// 	// t_ast	*node;
// 	// node = malloc(sizeof(t_ast));
// 	// node->type = NODE_COMMAND;
// 	// node->argv = malloc(3 * sizeof(char *));
// 	// node->argv[0] = ft_strdup("exit");
// 	// node->argv[1] = ft_strdup("5");
// 	// node->argv[2] = NULL;
// 	// node->redirs = NULL;
// 	// node->is_builtin = 1;
// 	// node->left = NULL;
// 	// node->right = NULL;
// 	// info.tree = node;
	
// 	// ls -l | grep command | sort | wc -l
// 	// t_ast *node_ls, *node_grep, *node_sort, *node_wc;
// 	// t_ast *pipe1, *pipe2, *pipe3;
// 	// node_ls = malloc(sizeof(t_ast));
// 	// node_ls->type = NODE_COMMAND;
// 	// node_ls->argv = malloc(3 * sizeof(char *));
// 	// node_ls->argv[0] = ft_strdup("ls");
// 	// node_ls->argv[1] = ft_strdup("-l");
// 	// node_ls->argv[2] = NULL;
// 	// node_ls->redirs = NULL;
// 	// node_ls->is_builtin = 0;
// 	// node_ls->left = NULL;
// 	// node_ls->right = NULL;
// 	// node_grep = malloc(sizeof(t_ast));
// 	// node_grep->type = NODE_COMMAND;
// 	// node_grep->argv = malloc(3 * sizeof(char *));
// 	// node_grep->argv[0] = ft_strdup("grep");
// 	// node_grep->argv[1] = ft_strdup("command");
// 	// node_grep->argv[2] = NULL;
// 	// node_grep->redirs = NULL;
// 	// node_grep->is_builtin = 0;
// 	// node_grep->left = NULL;
// 	// node_grep->right = NULL;
// 	// node_sort = malloc(sizeof(t_ast));
// 	// node_sort->type = NODE_COMMAND;
// 	// node_sort->argv = malloc(2 * sizeof(char *));
// 	// node_sort->argv[0] = ft_strdup("sort");
// 	// node_sort->argv[1] = NULL;
// 	// node_sort->redirs = NULL;
// 	// node_sort->is_builtin = 0;
// 	// node_sort->left = NULL;
// 	// node_sort->right = NULL;
// 	// node_wc = malloc(sizeof(t_ast));
// 	// node_wc->type = NODE_COMMAND;
// 	// node_wc->argv = malloc(3 * sizeof(char *));
// 	// node_wc->argv[0] = ft_strdup("wc");
// 	// node_wc->argv[1] = ft_strdup("-l");
// 	// node_wc->argv[2] = NULL;
// 	// node_wc->redirs = NULL;
// 	// node_wc->is_builtin = 0;
// 	// node_wc->left = NULL;
// 	// node_wc->right = NULL;
// 	// pipe1 = malloc(sizeof(t_ast));
// 	// pipe1->type = NODE_PIPE;
// 	// pipe1->argv = NULL;
// 	// pipe1->redirs = NULL;
// 	// pipe1->is_builtin = 0;
// 	// pipe1->left = node_ls;
// 	// pipe1->right = node_grep;
// 	// pipe2 = malloc(sizeof(t_ast));
// 	// pipe2->type = NODE_PIPE;
// 	// pipe2->argv = NULL;
// 	// pipe2->redirs = NULL;
// 	// pipe2->is_builtin = 0;
// 	// pipe2->left = pipe1;
// 	// pipe2->right = node_sort;
// 	// pipe3 = malloc(sizeof(t_ast));
// 	// pipe3->type = NODE_PIPE;
// 	// pipe3->argv = NULL;
// 	// pipe3->redirs = NULL;
// 	// pipe3->is_builtin = 0;
// 	// pipe3->left = pipe2;
// 	// pipe3->right = node_wc;
// 	// info.tree = pipe3;

// 	// echo <<EOF
// 	// t_ast *node_cat = malloc(sizeof(t_ast));
// 	// node_cat->type = NODE_COMMAND;
// 	// node_cat->argv = malloc(2 * sizeof(char *));
// 	// node_cat->argv[0] = ft_strdup("echo");
// 	// node_cat->argv[1] = NULL;
// 	// node_cat->redirs = malloc(sizeof(t_redir));
// 	// node_cat->redirs->type = REDIR_HEREDOC;
// 	// node_cat->redirs->target = ft_strdup("EOF");
// 	// node_cat->redirs->next = NULL;
// 	// node_cat->is_builtin = 1;
// 	// node_cat->left = node_cat->right = NULL;
// 	// info.tree = node_cat;
	
// 	// t_ast *node_grep = malloc(sizeof(t_ast));
// 	// node_grep->type = NODE_COMMAND;
// 	// node_grep->argv = malloc(3 * sizeof(char *));
// 	// node_grep->argv[0] = ft_strdup("grep");
// 	// node_grep->argv[1] = ft_strdup("Hello");
// 	// node_grep->argv[2] = NULL;
// 	// node_grep->redirs = malloc(sizeof(t_redir));
// 	// node_grep->redirs->type = REDIR_OUTPUT;
// 	// node_grep->redirs->target = ft_strdup("output.txt");
// 	// node_grep->redirs->next = NULL;
// 	// node_grep->is_builtin = 0;
// 	// node_grep->left = node_grep->right = NULL;
// 	// t_ast *pipe_node = malloc(sizeof(t_ast));
// 	// pipe_node->type = NODE_PIPE;
// 	// pipe_node->argv = NULL;
// 	// pipe_node->redirs = NULL;
// 	// pipe_node->is_builtin = 0;
// 	// pipe_node->left = node_cat;
// 	// pipe_node->right = node_grep;
// 	// info.tree = pipe_node;

// 	// // command: cat <<EOF > hello.txt
// 	// t_ast *node = malloc(sizeof(t_ast));
// 	// node->type = NODE_COMMAND;
// 	// node->argv = malloc(2 * sizeof(char *));
// 	// node->argv[0] = ft_strdup("cat");
// 	// node->argv[1] = NULL;
// 	// node->is_builtin = 0;
// 	// node->left = NULL;
// 	// node->right = NULL;
// 	// t_redir *r1 = malloc(sizeof(t_redir));
// 	// r1->type = REDIR_HEREDOC;
// 	// r1->target = ft_strdup("EOF");
// 	// r1->next = NULL;
// 	// t_redir *r2 = malloc(sizeof(t_redir));
// 	// r2->type = REDIR_OUTPUT;
// 	// r2->target = ft_strdup("hello.txt");
// 	// r2->next = NULL;
// 	// r1->next = r2;
// 	// node->redirs = r1;
// 	// info.tree = node;

// 	// // command: sleep | echo hello
// 	// t_ast *node = malloc(sizeof(t_ast));
// 	// node->type = NODE_COMMAND;
// 	// node->argv = malloc(2 * sizeof(char *));
// 	// node->argv[0] = ft_strdup("cat");
// 	// node->argv[1] = NULL;
// 	// node->is_builtin = 0;
// 	// node->left = NULL;
// 	// node->right = NULL;
// 	// t_redir *r1 = malloc(sizeof(t_redir));
// 	// r1->type = REDIR_HEREDOC;
// 	// r1->target = ft_strdup("EOF");
// 	// r1->next = NULL;
// 	// t_redir *r2 = malloc(sizeof(t_redir));
// 	// r2->type = REDIR_OUTPUT;
// 	// r2->target = ft_strdup("hello.txt");
// 	// r2->next = NULL;
// 	// r1->next = r2;
// 	// node->redirs = r1;
// 	// info.tree = node;
	
// 	// // Command: cat <<EOF | grep Hello > hello.txt
// 	// t_ast *left = malloc(sizeof(t_ast));
// 	// left->type = NODE_COMMAND;
// 	// left->argv = malloc(2 * sizeof(char *));
// 	// left->argv[0] = ft_strdup("cat");
// 	// left->argv[1] = NULL;
// 	// left->is_builtin = 0;
// 	// left->left = NULL;
// 	// left->right = NULL;
// 	// t_redir *r1 = malloc(sizeof(t_redir));
// 	// r1->type = REDIR_HEREDOC;
// 	// r1->target = ft_strdup("EOF");
// 	// r1->next = NULL;
// 	// left->redirs = r1;
// 	// t_ast *right = malloc(sizeof(t_ast));
// 	// right->type = NODE_COMMAND;
// 	// right->argv = malloc(3 * sizeof(char *));
// 	// right->argv[0] = ft_strdup("grep");
// 	// right->argv[1] = ft_strdup("Hello");
// 	// right->argv[2] = NULL;
// 	// right->is_builtin = 0;
// 	// right->left = NULL;
// 	// right->right = NULL;
// 	// t_redir *r2 = malloc(sizeof(t_redir));
// 	// r2->type = REDIR_OUTPUT;
// 	// r2->target = ft_strdup("hello.txt");
// 	// r2->next = NULL;
// 	// right->redirs = r2;
// 	// t_ast *root = malloc(sizeof(t_ast));
// 	// root->type = NODE_PIPE;
// 	// root->argv = NULL;
// 	// root->is_builtin = 0;
// 	// root->left = left;
// 	// root->right = right;
// 	// root->redirs = NULL;
// 	// info.tree = root;
	
// 	// echo hello > hello.txt | ls -l
// 	// t_ast *cmd1 = malloc(sizeof(t_ast));
// 	// cmd1->type = NODE_COMMAND;
// 	// cmd1->argv = malloc(3 * sizeof(char *));
// 	// cmd1->argv[0] = ft_strdup("echo");
// 	// cmd1->argv[1] = ft_strdup("hello");
// 	// cmd1->argv[2] = NULL;
// 	// cmd1->is_builtin = 1;
// 	// cmd1->left = NULL;
// 	// cmd1->right = NULL;
// 	// cmd1->redirs = NULL;
// 	// t_redir *redir1 = malloc(sizeof(t_redir));
// 	// redir1->type = REDIR_OUTPUT;
// 	// redir1->target = ft_strdup("hello.txt");
// 	// redir1->next = NULL;
// 	// cmd1->redirs = redir1;
// 	// t_ast *cmd2 = malloc(sizeof(t_ast));
// 	// cmd2->type = NODE_COMMAND;
// 	// cmd2->argv = malloc(3 * sizeof(char *));
// 	// cmd2->argv[0] = ft_strdup("ls");
// 	// cmd2->argv[1] = ft_strdup("-l");
// 	// cmd2->argv[2] = NULL;
// 	// cmd2->is_builtin = 0;
// 	// cmd2->left = NULL;
// 	// cmd2->right = NULL;
// 	// cmd2->redirs = NULL;
// 	// t_ast *pipe_node = malloc(sizeof(t_ast));
// 	// pipe_node->type = NODE_PIPE;
// 	// pipe_node->argv = NULL;
// 	// pipe_node->redirs = NULL;
// 	// pipe_node->is_builtin = 0;
// 	// pipe_node->left = cmd1;
// 	// pipe_node->right = cmd2;
// 	// info.tree = cmd1;

// 	// // exit | echo hello
// 	// t_ast *ls_node = malloc(sizeof(t_ast));
//     // t_ast *echo_node = malloc(sizeof(t_ast));
//     // t_redir *redir = malloc(sizeof(t_redir));
//     // ls_node->type = NODE_COMMAND;
//     // ls_node->argv = malloc(3 * sizeof(char *));
//     // ls_node->argv[0] = ft_strdup("ls");
//     // ls_node->argv[1] = ft_strdup("-l");
//     // ls_node->argv[2] = NULL;
//     // ls_node->redirs = NULL;
//     // ls_node->is_builtin = 0;
//     // ls_node->left = NULL;
//     // ls_node->right = NULL;
//     // echo_node->type = NODE_COMMAND;
//     // echo_node->argv = malloc(3 * sizeof(char *));
//     // echo_node->argv[0] = ft_strdup("echo");
//     // echo_node->argv[1] = ft_strdup("hello");
//     // echo_node->argv[2] = NULL;
//     // echo_node->is_builtin = 1;
// 	// echo_node->redirs = NULL;
//     // echo_node->left = NULL;
//     // echo_node->right = NULL;
//     // t_ast *pipe_node = malloc(sizeof(t_ast));
//     // pipe_node->type = NODE_PIPE;
//     // pipe_node->left = ls_node;
//     // pipe_node->right = echo_node;
//     // pipe_node->argv = NULL;
//     // pipe_node->redirs = NULL;
//     // info.tree = pipe_node;
	
// 	// echo < text.txt | sleep 5 | sleep 10 > text.txt
// 	// t_ast   *echo_node;
// 	// t_ast   *sleep5_node;
// 	// t_ast   *sleep10_node;
// 	// t_ast   *pipe1;
// 	// t_ast   *pipe2;
// 	// echo_node = malloc(sizeof(t_ast));
// 	// echo_node->type = NODE_COMMAND;
// 	// echo_node->argv = malloc(2 * sizeof(char *));
// 	// echo_node->argv[0] = ft_strdup("echo");
// 	// echo_node->argv[1] = NULL;
// 	// echo_node->is_builtin = 1;
// 	// echo_node->left = NULL;
// 	// echo_node->right = NULL;
// 	// echo_node->redirs = malloc(sizeof(t_redir));
// 	// echo_node->redirs->type = REDIR_INPUT;
// 	// echo_node->redirs->target = ft_strdup("text.txt");
// 	// echo_node->redirs->next = NULL;
// 	// sleep5_node = malloc(sizeof(t_ast));
// 	// sleep5_node->type = NODE_COMMAND;
// 	// sleep5_node->argv = malloc(3 * sizeof(char *));
// 	// sleep5_node->argv[0] = ft_strdup("sleep");
// 	// sleep5_node->argv[1] = ft_strdup("5");
// 	// sleep5_node->argv[2] = NULL;
// 	// sleep5_node->is_builtin = 0;
// 	// sleep5_node->left = NULL;
// 	// sleep5_node->right = NULL;
// 	// sleep5_node->redirs = NULL;
// 	// sleep10_node = malloc(sizeof(t_ast));
// 	// sleep10_node->type = NODE_COMMAND;
// 	// sleep10_node->argv = malloc(3 * sizeof(char *));
// 	// sleep10_node->argv[0] = ft_strdup("sleep");
// 	// sleep10_node->argv[1] = ft_strdup("10");
// 	// sleep10_node->argv[2] = NULL;
// 	// sleep10_node->is_builtin = 0;
// 	// sleep10_node->left = NULL;
// 	// sleep10_node->right = NULL;
// 	// sleep10_node->redirs = malloc(sizeof(t_redir));
// 	// sleep10_node->redirs->type = REDIR_OUTPUT;
// 	// sleep10_node->redirs->target = ft_strdup("text.txt");
// 	// sleep10_node->redirs->next = NULL;
// 	// pipe1 = malloc(sizeof(t_ast));
// 	// pipe1->type = NODE_PIPE;
// 	// pipe1->left = echo_node;
// 	// pipe1->right = sleep5_node;
// 	// pipe2 = malloc(sizeof(t_ast));
// 	// pipe2->type = NODE_PIPE;
// 	// pipe2->left = pipe1;
// 	// pipe2->right = sleep10_node;
// 	// info.tree = pipe2;

// 	// Execute
// 	executor(info.tree, &info);
// 	while (1)
// 	{
// 		char * line = readline("minishell$ ");
// 		if (!line) // Ctrl+D
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (*line)
// 			add_history(line);
// 		//info.tree = parse(line);
// 		free(line);
// 		if (info.tree)
// 			executor(info.tree, &info);
// 		clean_loop(&info);
// 	}
// 	printf("Exit status: %d\n", info.last_status);
// 	clean_shell(&info);
// }
