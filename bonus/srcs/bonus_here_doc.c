/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_here_doc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 18:20:59 by atudor            #+#    #+#             */
/*   Updated: 2024/01/13 18:21:08 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"
#include "../../libft/libft.h"
#include "../includes/get_next_line/get_next_line.h"

int	here_doc_function(char **argv, char **env)
{
	pid_t	pid1;
	pid_t	pid2;
	int		pipefd[2];
	int		pipefd1[2];

	pipe_error(pipefd);
	pipe_error(pipefd1);
	write_in_pipe(pipefd, argv);
	close(pipefd[1]);
	pid1 = fork();
	if (pid1 == 0)
		execute_first_command_here_doc(pipefd, pipefd1, argv[3], env);
	pid2 = fork();
	if (pid2 == 0)
		execute_last_command_here_doc(pipefd1, argv[4], env, argv[5]);
	close_fds(2, pipefd[0], pipefd[1]);
	close_fds(2, pipefd1[0], pipefd1[1]);
	return (wait_for_process(pid2));
}

void	write_in_pipe(int *pipefd, char **argv)
{
	char	*line;

	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		line = get_next_line(0);
		if (line == NULL)
			break ;
		if (ft_strlen(line) >= ft_strlen(argv[2])
			&& ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
}

void	execute_first_command_here_doc(int *f1, int *f2, char *av, char **env)
{
	dup2(f1[0], 0);
	close(f1[0]);
	dup2(f2[1], 1);
	close(f2[1]);
	execute_command(env, av);
	exit(EXIT_FAILURE);
}

void	execute_last_command_here_doc(int *fd, char *av, char **env, char *out)
{
	int	fd1;

	fd1 = open(out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd1 < 0)
	{
		perror(out);
		exit(EXIT_FAILURE);
	}
	dup2(fd1, 1);
	close(fd1);
	dup2(fd[0], 0);
	close(fd[0]);
	close(fd[1]);
	execute_command(env, av);
	exit(EXIT_FAILURE);
}
