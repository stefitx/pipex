/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:39:12 by atudor            #+#    #+#             */
/*   Updated: 2023/12/11 15:39:51 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <fcntl.h>
# include "../libft/libft.h"

void	execute_first_command(int **pipefd, char *command, char *infile, char **env);
char	**find_path(char **env);
char	*access_path(char **env, char *argv);
char	**find_command(char *argv);
char	**undo_split(char **array);
void	close_fds(int nr_fds, ...);
pid_t	create_fork(pid_t pid);
void	execute_command(char **env, char *command);
void	execute_middle_command(int **pipefd, char *argv, char **env, int j);
int	wait_for_process(pid_t pid);
void	validate_args(int argc);
void	pipe_error(int *pipefd1);

#endif
