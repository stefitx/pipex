/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 21:20:06 by atudor            #+#    #+#             */
/*   Updated: 2023/12/21 21:20:10 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

void	validate_args(int argc)
{
	if (argc != 5)
	{
		write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 38);
		exit(EXIT_FAILURE);
	}
}

void	pipe_error(int pipefd1[2], int pipefd2[2])
{
	if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1)
	{
		write(2, "Error creating pipe", 19);
		exit(EXIT_FAILURE);
	}
}

void	execute_command(char **env, char *command)
{
	char	**cmd;
	char	*path;

	cmd = find_command(command);
	path = access_path(env, command);
	if (execve(path, cmd, env) == -1)
	{
		perror("Command execution failed");
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
