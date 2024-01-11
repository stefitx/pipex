/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:09:48 by atudor            #+#    #+#             */
/*   Updated: 2023/12/11 16:11:01 by atudor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"
#include "../includes/get_next_line/get_next_line.h"
#include "../../libft/libft.h"

// void	execute_middle_command(int **pipefd, char *argv, char **env, int j)
// {
// 	dup2(pipefd[j][1], 1);
// 	dup2(pipefd[j - 1][0], 0);
// 	close(pipefd[j][1]);
// 	close(pipefd[j - 1][0]);
// 	execve(access_path(env, argv), find_command(argv), env);
// 	exit(EXIT_FAILURE);
// }	

// void	execute_last_command(int **pipefd, char *argv, char **env, char *outfile, int pipe_num)
// {
// 	int	fd;

// 	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 	{
// 		perror(outfile);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (dup2(fd, 1) < 0)
// 	{
// 		write(2, "Error duplicating file descriptor", 33);
// 		close(fd);
// 		exit(EXIT_FAILURE);
// 	}
// 	close(fd);
// 	dup2(pipefd[pipe_num - 1][0], 0);
// 	close(pipefd[pipe_num - 1][0]);
// 	close(pipefd[pipe_num - 1][1]);
// 	execve(access_path(env, argv), find_command(argv), env);
// 	exit(EXIT_FAILURE);
// }

// void	execute_first_command(int **pipefd, char **argv, char **env)
// {
// 	int	fd;

// 	fd = open(argv[1], O_RDONLY);
// 	if (access(argv[1], R_OK) == -1)
// 	{
// 		perror(argv[1]);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (fd < 0)
// 	{
// 		ft_putstr_fd("zsh: no such file or directory: ", 2);
// 		ft_putstr_fd(argv[1], 2);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (dup2(fd, 0) < 0)
// 	{
// 		close(fd);
// 		exit(EXIT_FAILURE);
// 	}
// 	close(fd);
// 	dup2(pipefd[0][1], 1);
// 	//close_fds(2, pipefd[0], pipefd[1]);
// 	execute_command(env, argv[2]);
// 	close_fds(2, 0, 1);
// 	exit(EXIT_FAILURE);
// }

// int	main(int argc, char **argv, char **env)
// {
// 	int		**pipefd;
// 	int		pipe_num;
// 	int		commands;
// 	pid_t	*pid;

// 	validate_args(argc);
// 	commands = argc - 3;
// 	pipe_num = argc - 4;
// 		pipefd = malloc(sizeof(int *) * pipe_num);

// 	int i = 0;
// 	while (i < pipe_num)
// 	{
// 		pipefd[i] = malloc(sizeof(int) * 2);
// 		pipe_error(pipefd[i]);
// 		i++;
// 	}
// 	pid = (pid_t *)malloc(sizeof(pid_t) * commands);
// 	int j = 0;
// 	while (j < commands)
// 	{
// 		pid[j] = fork();
// 		if (pid[j] == 0)
// 		{
// 			if (j == 0)
// 				execute_first_command(pipefd, argv, env);
// 			else if (j == commands - 1)
// 			{
// 				printf("in last command, j is: %d\n", j);
// 				printf("pipe_num - 1 is: %d\n", pipe_num - 1);
// 				fflush(stdout);
// 				execute_last_command(pipefd, argv[argc - 2], env, argv[argc - 1], pipe_num);
// 			}
// 			else
// 			{
// 				printf("in middle command j is: %d\n", j);
// 				fflush(stdout);
// 				execute_middle_command(pipefd, argv[j + 2], env, j);
// 			}
				
// 				//close(pipefd[j][1]);
// 		}
// 		// else
// 		// {
// 		// 	// In the parent, close the ends of the current pipe that are not needed
// 		// 	if (j > 0)
// 		// 	{
// 		// 		close(pipefd[j - 1][0]);  // Close the previous read end
// 		// 	}
// 		// 	if (j < commands - 1)
// 		// 	{
// 		// 		close(pipefd[j][1]);  // Close the current write end
// 		// 	}
// 		// }
// 		j++;
// 	}
// 	return (wait_for_process(pid[j]));
// }

void execute_middle_command(int **pipefd, char *argv, char **env, int j) {
    dup2(pipefd[j - 1][0], 0); // Read from the previous pipe
    close(pipefd[j - 1][0]);
    dup2(pipefd[j][1], 1); // Write to the next pipe
    close(pipefd[j][1]);
    execve(access_path(env, argv), find_command(argv), env);
    exit(EXIT_FAILURE);
}

void execute_last_command(int **pipefd, char *argv, char **env, char *outfile, int pipe_num) {
    int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror(outfile);
        exit(EXIT_FAILURE);
    }
    dup2(fd, 1);
    close(fd);
    dup2(pipefd[pipe_num - 1][0], 0);
    close(pipefd[pipe_num - 1][0]);
    close(pipefd[pipe_num - 1][1]);
    execve(access_path(env, argv), find_command(argv), env);
    exit(EXIT_FAILURE);
}

void execute_first_command(int **pipefd, char *command, char *infile, char **env) {
    int fd = open(infile, O_RDONLY);
    if (fd < 0) {
        perror(infile);
        exit(EXIT_FAILURE);
    }
    dup2(fd, 0);
    close(fd);
    dup2(pipefd[0][1], 1);
    close(pipefd[0][1]);
    execve(access_path(env, command), find_command(command), env);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **env) {
    int **pipefd;
    int pipe_num;
    int commands;
    pid_t *pid = NULL;

    validate_args(argc);
    if (ft_strncmp("here_doc", argv[1], 8) == 0)
    {
    	pipefd = malloc(sizeof(int *) * 2);
    	for (int i = 0; i < 2; i++) {
        pipefd[i] = malloc(sizeof(int) * 2);
        pipe_error(pipefd[i]);
    }
    	pipe_error(pipefd[0]);
    	char * line;
    	while (ft_strncmp(line, argv[2], ft_strlen(line)) != 0)
    	{
			line = get_next_line(0);
			write(pipefd[0][1], line, ft_strlen(line));
    	}
    	pid[0] = fork();
    	if (pid[0] == 0)
    	{
    		execute_middle_command(pipefd, argv[3], env, 1);
    	}
    		else
		{
			wait(NULL);
			execute_last_command(pipefd, argv[4], env, argv[5], 1);
		}
		return 0;
    }
    commands = argc - 3;
    pipe_num = commands - 1;
    pipefd = malloc(sizeof(int *) * pipe_num);

    for (int i = 0; i < pipe_num; i++) {
        pipefd[i] = malloc(sizeof(int) * 2);
        pipe_error(pipefd[i]);
    }

    pid = (pid_t *)malloc(sizeof(pid_t) * commands);
    for (int j = 0; j < commands; j++) {
        if (j < commands - 1) {
            pipe_error(pipefd[j]);
        }

        pid[j] = fork();
        if (pid[j] == 0) { // Child process
            if (j == 0) {
                execute_first_command(pipefd, argv[2], argv[1], env);
            } else if (j == commands - 1) {
                execute_last_command(pipefd, argv[argc - 2], env, argv[argc - 1], pipe_num);
            } else {
                execute_middle_command(pipefd, argv[j + 2], env, j);
            }
            exit(EXIT_FAILURE); // Ensure child process exits after execution
        } else { // Parent process
            // Close the write end of the current pipe, if not the last command
            if (j < commands - 1) {
                close(pipefd[j][1]);
            }
        }
    }

    // Parent process: Close all read ends and wait for child processes
    for (int j = 0; j < pipe_num; j++) {
        close(pipefd[j][0]);
        waitpid(pid[j], NULL, 0);
    }

    // Free memory and close file descriptors
    for (int i = 0; i < pipe_num; i++) {
        free(pipefd[i]);
    }
    free(pipefd);
    free(pid);

    return 0;
}
