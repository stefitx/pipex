/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:09:48 by atudor            #+#    #+#             */
/*   Updated: 2023/12/11 16:11:01 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

void	execute_second_command(int *pipefd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(argv[4]);
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
	exit(EXIT_FAILURE);
}

void	execute_first_command(int *pipefd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (access(argv[1], R_OK) == -1)
	{
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
	if (fd < 0)
	{
		ft_putstr_fd("zsh: no such file or directory: ", 2);
		ft_putstr_fd(argv[1], 2);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, 0) < 0)
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	dup2(pipefd[1], 1);
	close_fds(2, pipefd[0], pipefd[1]);
	execute_command(env, argv[2]);
	close_fds(2, 0, 1);
	exit(EXIT_FAILURE);
}

int	main(int argc, char **argv, char **env)
{
	int		pipefd1[2];
	pid_t	pid1;
	pid_t	pid2;

	validate_args(argc);
	pipe_error(pipefd1, pipefd1);
	pid1 = fork();
	if (pid1 == 0)
	{
		close_fds(1, pipefd1[0]);
		execute_first_command(pipefd1, argv, env);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd1[0], 0);
		close_fds(2, pipefd1[1], pipefd1[0]);
		execute_second_command(pipefd1, argv, env);
	}
	close_fds(2, pipefd1[0], pipefd1[1]);
	return (wait_for_process(pid2));
}
