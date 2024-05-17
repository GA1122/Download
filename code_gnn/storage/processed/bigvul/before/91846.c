void log_error_mesg(const char *file, int line, const char *mesg)
{
    int errno_save = errno;
    fprintf(stderr, "%s%s:%d - ", get_commonlog_time(), file, line);
    errno = errno_save;
    perror(mesg);
    errno = errno_save;
}
