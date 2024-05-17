int open_pipe_fd(const char *command)
{
    int pipe_fds[2];
    int pid;
     
    if (pipe(pipe_fds) == -1)
        return -1;
    pid = fork();
    if (pid == 0) {              
        close(pipe_fds[1]);
        if (pipe_fds[0] != 0) {
            dup2(pipe_fds[0], 0);
            close(pipe_fds[0]);
        }
        execl("/bin/sh", "sh", "-c", command, (char *) 0);
        exit(EXIT_FAILURE);
    }
    close(pipe_fds[0]);
    if (pid < 0) {
        close(pipe_fds[1]);
        return -1;
    }
    return pipe_fds[1];
}
