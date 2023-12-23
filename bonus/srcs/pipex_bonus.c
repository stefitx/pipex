/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:09:48 by atudor            #+#    #+#             */
/*   Updated: 2023/12/11 16:11:01 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

void	validate_args(int argc, char **argv)
{
	(void)argv;
	if (argc < 5)
	{
		write(1, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 38);
		exit(EXIT_FAILURE);
	}
}

void	pipe_error(int pipefd1[2], int pipefd2[2])
{
	if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1)
	{
		perror("Error creating pipe");
		exit(EXIT_FAILURE);
	}
}

void	execute_second_command(int *pipefd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Error in opening the output file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, 1) < 0)
	{
		write(2, "Error duplicating file descriptor", 33);
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	dup2(pipefd[0], 0);
	close(pipefd[0]);
	close(pipefd[1]);
	execve(access_path(env, argv[3]), find_command(argv[3]), env);
	perror("Error executing second command");
	exit(EXIT_FAILURE);
}

void	execute_first_command(int *pipefd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Error in opening the input file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, 0) < 0)
	{
		perror("Error duplicating file descriptor");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	dup2(pipefd[1], 1);
	close(pipefd[0]);
	close(pipefd[1]);
	execve(access_path(env, argv[2]), find_command(argv[2]), env);
	perror("Error executing first command");
	return (close(0), close(1), exit(EXIT_FAILURE));
}

int	main(int argc, char **argv, char **env)
{
	int		pipefd1[2];
	int		pipefd2[2];
	pid_t	pid1;
	pid_t	pid2;

	validate_args(argc, argv);
	pipe_error(pipefd1, pipefd2);
	pid1 = fork();
	if (pid1 == 0)
	{
		close_fds(3, pipefd1[0], pipefd2[0], pipefd2[1]);
		execute_first_command(pipefd1, argv, env);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd1[0], 0);
		close_fds(2, pipefd1[1], pipefd2[0]);
		execute_second_command(pipefd2, argv, env);
	}
	close_fds(4, pipefd1[0], pipefd1[1], pipefd2[0], pipefd2[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
