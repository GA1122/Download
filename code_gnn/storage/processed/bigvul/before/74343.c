static char* get_executable(pid_t pid, int *fd_p)
{
    char buf[sizeof("/proc/%lu/exe") + sizeof(long)*3];

    sprintf(buf, "/proc/%lu/exe", (long)pid);
    if (fd_p)
        *fd_p = open(buf, O_RDONLY);  
    char *executable = malloc_readlink(buf);
    if (!executable)
        return NULL;
     
    char *deleted = executable + strlen(executable) - strlen(" (deleted)");
    if (deleted > executable && strcmp(deleted, " (deleted)") == 0)
    {
        *deleted = '\0';
        log("File '%s' seems to be deleted", executable);
    }
     
    char *prelink = executable + strlen(executable) - strlen(".#prelink#.XXXXXX");
    if (prelink > executable && strncmp(prelink, ".#prelink#.", strlen(".#prelink#.")) == 0)
    {
        log("File '%s' seems to be a prelink temporary file", executable);
        *prelink = '\0';
    }
    return executable;
}
