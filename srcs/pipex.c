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
	int fd;
(void)env;
	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Error in opening the output file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, 1) < 0) 
	{
        perror("Error duplicating file descriptor");
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
	int fd;
	(void)env;
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
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **env)
{
	int	pipefd[2];
	pid_t	pid;
	find_path(env);
	if (argc != 5)
	{
		write(1, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 38);
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == 0)
		execute_first_command(pipefd, argv, env);
	else
	{
		wait(0);
		execute_second_command(pipefd, argv, env);
	}
	return 0;
}