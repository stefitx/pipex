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

#include "../includes/pipex.h"
#include "../libft/libft.h"

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

char	*access_path(char **env, char *argv)
{
	int		i;
	char	*path;
	char	**split_path;
	char	**command;

	split_path = find_path(env);
	command = find_command(argv);
	i = 0;
	while (split_path[i])
	{
		path = ft_strjoin(ft_strjoin(split_path[i], "/"), command[0]);
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) == -1)
				return (NULL);
			return (path);
		}
		free(path);
		i++;
	}
	return (NULL);
}
