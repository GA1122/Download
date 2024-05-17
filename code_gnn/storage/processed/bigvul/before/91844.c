void log_error_doc(request * req)
{
    int errno_save = errno;
    char *escaped_pathname;

    if (virtualhost) {
        fprintf(stderr, "%s ", req->local_ip_addr);
    } else if (vhost_root) {
        fprintf(stderr, "%s ", (req->host ? req->host : "(null)"));
    }
    escaped_pathname = escape_pathname(req->pathname);
    print_remote_ip (req, stderr);
    if (vhost_root) {
        fprintf(stderr, " - - %srequest [%s] \"%s\" (\"%s\"): ",
                get_commonlog_time(),
                (req->header_host ? req->header_host : "(null)"),
                (req->logline ? req->logline : "(null)"),
                (escaped_pathname ? escaped_pathname : "(null)"));
    } else {
        fprintf(stderr, " - - %srequest \"%s\" (\"%s\"): ",
                get_commonlog_time(),
                (req->logline ? req->logline : "(null)"),
                (escaped_pathname ? escaped_pathname : "(null)"));
    }
    free(escaped_pathname);

    errno = errno_save;
}
