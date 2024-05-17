void log_error_time(void)
{
    int errno_save = errno;
    fputs(get_commonlog_time(), stderr);
    errno = errno_save;
}
