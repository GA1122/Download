int open_gen_fd(const char *spec)
{
    int fd;
    if (*spec == '|') {
        fd = open_pipe_fd(spec + 1);
    } else if (*spec == ':') {
        fd = open_net_fd(spec + 1);
    } else {
        fd = open(spec,
                  O_WRONLY | O_CREAT | O_APPEND,
                  S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP);
    }
    return fd;
}
