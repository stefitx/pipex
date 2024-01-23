/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:09:48 by atudor            #+#    #+#             */
/*   Updated: 2024/01/13 18:48:08 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"
#include "../includes/get_next_line/get_next_line.h"
#include "../../libft/libft.h"

void	execute_middle_command(int **pipefd, char *argv, char **env, int j)
{
	dup2(pipefd[j - 1][0], 0);
	close(pipefd[j - 1][0]);
	dup2(pipefd[j][1], 1);
	close(pipefd[j][1]);
	execve(access_path(env, argv), find_command(argv), env);
	exit(EXIT_FAILURE);
}

void	execute_last_command(int **pipefd, char **argv, char **env, int argc)
{
	int	fd;
	int	pipe_num;

	pipe_num = argc - 4;
	fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(argv[argc - 1]);
		exit(EXIT_FAILURE);
	}
	dup2(fd, 1);
	close(fd);
	dup2(pipefd[pipe_num - 1][0], 0);
	close(pipefd[pipe_num - 1][0]);
	close(pipefd[pipe_num - 1][1]);
	execve(access_path(env, argv[argc - 2]), find_command(argv[argc - 2]), env);
	exit(EXIT_FAILURE);
}

void	execute_first_command(int **fd, char *command, char *in, char **env)
{
	int	fd1;

	fd1 = open(in, O_RDONLY);
	if (fd1 < 0)
	{
		perror(in);
		exit(EXIT_FAILURE);
	}
	dup2(fd1, 0);
	close(fd1);
	dup2(fd[0][1], 1);
	close(fd[0][1]);
	execve(access_path(env, command), find_command(command), env);
	exit(EXIT_FAILURE);
}

pid_t	*do_commands(int argc, char **argv, char **env, int **pipefd)
{
	pid_t	*pid;
	int		j;

	pid = (pid_t *)malloc(sizeof(pid_t) * (argc - 3));
	j = 0;
	while (j < (argc - 3))
	{
		create_pipes_and_close_them(j, argc, pipefd, '1');
		pid[j] = fork();
		if (pid[j] == 0)
		{
			if (j == 0)
				execute_first_command(pipefd, argv[2], argv[1], env);
			else if (j == (argc - 3) - 1)
				execute_last_command(pipefd, argv, env, argc);
			else
				execute_middle_command(pipefd, argv[j + 2], env, j);
		}
		else
			create_pipes_and_close_them(j, argc, pipefd, '2');
		j++;
	}
	return (pid);
}

int	main(int argc, char **argv, char **env)
{
	int		**pipefd;
	pid_t	*pid;
	int		exit_status;

	if (validate_args(argc, argv) != 0)
		return (here_doc_function(argv, env));
	pipefd = malloc(sizeof(int *) * (argc - 4));
	pid = NULL;
	malloc_and_free(pipefd, pid, '1', argc);
	pid = do_commands(argc, argv, env, pipefd);
	exit_status = wait_for_process(pid[argc - 4]);
	malloc_and_free(pipefd, pid, '2', argc);
	return (exit_status);
}
