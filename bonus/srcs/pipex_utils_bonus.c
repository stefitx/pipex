/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:11:16 by atudor            #+#    #+#             */
/*   Updated: 2023/12/11 16:11:31 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"
#include "../../libft/libft.h"

char	**find_path(char **env)
{
	int		i;
	char	*to_split;
	char	**split_path;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	to_split = ft_substr(env[i], 5, ft_strlen(env[i]));
	split_path = ft_split(to_split, ':');
	free(to_split);
	return (split_path);
}

char	**find_command(char *argv)
{
	char	**command;

	command = ft_split(argv, ' ');
	return (command);
}

char	*construct_command_path(char **split_path, char *command)
{
	char	*path;
	int		i;
	char	*temp;

	i = 0;
	while (split_path[i])
	{
		temp = ft_strjoin(split_path[i], "/");
		path = ft_strjoin(temp, command);
		free(temp);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

char	*access_path(char **env, char *argv)
{
	char	*path;
	char	**split_path;
	char	**command;

	split_path = find_path(env);
	command = find_command(argv);
	if (access(command[0], F_OK) == 0)
	{
		if (access(command[0], X_OK) == -1)
		{
			ft_putstr_fd(command[0], 2);
			ft_putstr_fd(": Permission denied", 2);
			exit(126);
		}
		return (command[0]);
	}
	path = construct_command_path(split_path, command[0]);
	if (path != NULL)
		return (path);
	ft_putstr_fd("zsh: ", 2);
	ft_putstr_fd(command[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
	return (NULL);
}
