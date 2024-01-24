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

void	execute_first_command(int **fd, char *command, char *in, char **env);
char	**find_path(char **env);
char	*access_path(char **env, char *argv);
char	**find_command(char *argv);
char	**undo_split(char **array);
void	close_fds(int nr_fds, ...);
pid_t	create_fork(pid_t pid);
void	execute_command(char **env, char *command);
void	execute_middle_command(int **pipefd, char *argv, char **env, int j);
int		wait_for_process(pid_t pid);
int		validate_args(int argc, char **argv);
void	pipe_error(int *pipefd1);
void	execute_first_command_here_doc(int *f1, int *f2, char *av, char **env);
void	execute_last_command_here_doc(int *fd, char *av, char **env, char *out);
int		here_doc_function(char **argv, char **env);
void	write_in_pipe(int *pipefd, char **argv);
void	malloc_and_free(int **pipefd, pid_t *pid, char check, int argc);
void	create_pipes_and_close_them(int j, int argc, int **pipefd, char check);
void	free_matrixes(char **split_path, char **command);

#endif
