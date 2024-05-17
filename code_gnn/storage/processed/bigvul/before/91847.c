void log_error_mesg_fatal(const char *file, int line, const char *func, const char *mesg)
{
    int errno_save = errno;
    fprintf(stderr, "%s%s:%d (%s) - ", get_commonlog_time(), file, line, func);
    errno = errno_save;
    perror(mesg);
    exit(EXIT_FAILURE);
}