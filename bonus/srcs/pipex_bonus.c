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

void execute_middle_command(int **pipefd, char *argv, char **env, int j)
{
    dup2(pipefd[j - 1][0], 0); // Read from the previous pipe
    close(pipefd[j - 1][0]);
    dup2(pipefd[j][1], 1); // Write to the next pipe
    close(pipefd[j][1]);
    execve(access_path(env, argv), find_command(argv), env);
    exit(EXIT_FAILURE);
}

void execute_last_command(int **pipefd, char *argv, char **env, char *outfile, int pipe_num)
{
    int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
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

void execute_first_command(int **pipefd, char *command, char *infile, char **env)
{
    int fd = open(infile, O_RDONLY);
    if (fd < 0)
    {
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

int here_doc_function(char **argv, char **env)
{
    pid_t pid1;
    int pipefd[2];
    int pipefd1[2];
    char    *line;

    if (ft_strncmp("here_doc", argv[1], 8) == 0)
    {
        pipe_error(pipefd);
        pipe_error(pipefd1);
        while (1) 
        {
            write(1, "pipe heredoc> ", 14);
            line = get_next_line(0);
            if (line == NULL)
                break;
            if (ft_strlen(line) >= ft_strlen(argv[2]) && ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0)
            {
                free(line);
                break;
            }
            write(pipefd[1], line, ft_strlen(line));
            write(pipefd[1], "\n", 1);
            free(line);
        }
        close(pipefd[1]);
        pid1 = fork();
        if (pid1 == 0)
        {
            execute_first_command_here_doc(pipefd, pipefd1, argv[3], env);
            close(pipefd[0]);
            close(pipefd[1]);
        }
        else
        {
            waitpid(pid1, NULL, 0);
            execute_last_command_here_doc(pipefd1, argv[4], env, argv[5]);
            close(pipefd1[0]);
            close(pipefd1[1]);
        }
        return (1);
    }
    else
        return (0);
}

int main(int argc, char **argv, char **env) 
{
    int **pipefd = NULL;
    int pipe_num;
    int commands;
    pid_t *pid;
    int j;

    validate_args(argc);
    if (here_doc_function(argv, env) == 1)
        return 0;
    commands = argc - 3;
    pipe_num = commands - 1;
    pipefd = malloc(sizeof(int *) * pipe_num);
    j = 0;
    while (j < pipe_num)
    {
        pipefd[j] = malloc(sizeof(int) * 2);
        pipe_error(pipefd[j]);
        j++;
    }
    pid = (pid_t *)malloc(sizeof(pid_t) * commands);
    j = 0;
    while (j < commands)
    {
        if (j < commands - 1)
            pipe_error(pipefd[j]);
        pid[j] = fork();
        if (pid[j] == 0)
        {
            if (j == 0)
                execute_first_command(pipefd, argv[2], argv[1], env);
            else if (j == commands - 1)
                execute_last_command(pipefd, argv[argc - 2], env, argv[argc - 1], pipe_num);
            else
                execute_middle_command(pipefd, argv[j + 2], env, j);
        }
        else 
        {
            if (j < commands - 1)
                close(pipefd[j][1]);
        }
        j++;
    }
    j = 0;
    while (j < pipe_num)
    {
        close(pipefd[j][0]);
        waitpid(pid[j], NULL, 0);
        j++;
    }
    j = 0;
    while (j < pipe_num)
        free(pipefd[j++]);
    free(pipefd);
    free(pid);
    return (0);
}
