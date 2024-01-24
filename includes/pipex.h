/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:39:12 by atudor            #+#    #+#             */
/*   Updated: 2023/12/11 15:39:51 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <fcntl.h>
# include "../libft/libft.h"

void	execute_first_command(int *pipefd, char **argv, char **env);
char	**find_path(char **env);
char	*access_path(char **env, char *argv);
char	**find_command(char *argv);
char	**undo_split(char **array);
void	close_fds(int nr_fds, ...);
void	validate_args(int argc);
void	pipe_error(int pipefd1[2], int pipefd2[2]);
void	execute_command(char **env, char *command);
int		wait_for_process(pid_t pid);
char	**mod_split(char *s, char c);
void	free_matrixes(char **split_path, char **command);

#endif
