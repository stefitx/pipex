/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 01:57:07 by atudor            #+#    #+#             */
/*   Updated: 2023/12/31 01:57:23 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"
#include "../../libft/libft.h"

int	validate_args(int argc, char **argv)
{
	if (argc < 5)
	{
		write(2, "Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2 ", 46);
		write(2, "OR ./pipex here_doc LIMITER cmd cmd1 file\n", 43);
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp("here_doc", argv[1], 8) == 0)
		return (1);
	return (0);
}

void	pipe_error(int *pipefd1)
{
	if (pipe(pipefd1) == -1)
	{
		write(2, "Error creating pipe", 19);
		exit(EXIT_FAILURE);
	}
}

int	wait_for_process(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		return (WEXITSTATUS(status));
	}
	return (0);
}

void	malloc_and_free(int **pipefd, pid_t *pid, char check, int argc)
{
	int	j;

	j = 0;
	if (check == '1')
	{
		while (j < (argc - 4))
		{
			pipefd[j] = malloc(sizeof(int) * 2);
			pipe_error(pipefd[j]);
			j++;
		}
	}
	if (check == '2')
	{
		while (j < (argc - 4))
		{
			close(pipefd[j++][0]);
			waitpid(pid[j++], NULL, 0);
		}
		j = 0;
		while (j < (argc - 4))
			free(pipefd[j++]);
		free(pipefd);
		free(pid);
	}
}

void	create_pipes_and_close_them(int j, int argc, int **pipefd, char check)
{
	if (check == '1')
	{
		if (j < (argc - 3) - 1)
			pipe_error(pipefd[j]);
	}
	if (check == '2')
	{
		if (j < (argc - 3) - 1)
			close(pipefd[j][1]);
	}
}
