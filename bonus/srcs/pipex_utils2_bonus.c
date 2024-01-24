/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:35:24 by atudor            #+#    #+#             */
/*   Updated: 2024/01/24 15:35:27 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"
#include "../../libft/libft.h"

void	close_fds(int nr_fds, ...)
{
	va_list	args;
	int		i;
	int		fd;

	i = 0;
	if (nr_fds <= 0)
		return ;
	va_start(args, nr_fds);
	while (i < nr_fds)
	{
		fd = va_arg(args, int);
		if (fd >= 0)
			close(fd);
		i++;
	}
	va_end(args);
}

void	execute_command(char **env, char *command)
{
	char	**cmd;
	char	*path;
	int		i;

	cmd = find_command(command);
	i = 0;
	while (cmd[i] != NULL)
		i++;
	path = access_path(env, command);
	if (execve(path, cmd, env) == -1)
	{
		free_matrix(cmd, i);
		free(path);
		perror("Command execution failed");
		exit(EXIT_FAILURE);
	}
}

void	free_matrixes(char **split_path, char **command)
{
	int	i;

	i = 0;
	while (split_path[i] != NULL)
		i++;
	free_matrix(split_path, i);
	i = 0;
	while (command[i] != NULL)
		i++;
	free_matrix(command, i);
}
