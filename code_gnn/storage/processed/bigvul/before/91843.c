void log_error(const char *mesg)
{
    fprintf(stderr, "%s%s", get_commonlog_time(), mesg);
}
