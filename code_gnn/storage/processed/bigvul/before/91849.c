void open_logs(void)
{
    int access_log;

     
     
     
    if (error_log_name) {
        int error_log;

         
        error_log = open_gen_fd(error_log_name);
        if (error_log < 0) {
            DIE("unable to open error log");
        }

         
        if (dup2(error_log, STDERR_FILENO) == -1) {
            DIE("unable to dup2 the error log");
        }
        close(error_log);
    }

    if (access_log_name) {
        access_log = open_gen_fd(access_log_name);
    } else {
        access_log = open("/dev/null", 0);
    }
    if (access_log < 0) {
        DIE("unable to open access log");
    }

    if (dup2(access_log, STDOUT_FILENO) == -1) {
        DIE("can't dup2 /dev/null to STDOUT_FILENO");
    }
    if (fcntl(access_log, F_SETFD, 1) == -1) {
        DIE("unable to set close-on-exec flag for access_log");
    }

    close(access_log);

    if (cgi_log_name) {
        cgi_log_fd = open_gen_fd(cgi_log_name);
        if (cgi_log_fd == -1) {
            WARN("open cgi_log");
            free(cgi_log_name);
            cgi_log_name = NULL;
            cgi_log_fd = 0;
        } else {
            if (fcntl(cgi_log_fd, F_SETFD, 1) == -1) {
                WARN("unable to set close-on-exec flag for cgi_log");
                free(cgi_log_name);
                cgi_log_name = NULL;
                close(cgi_log_fd);
                cgi_log_fd = 0;
            }
        }
    }
#ifdef SETVBUF_REVERSED
    setvbuf(stderr, _IONBF, (char *) NULL, 0);
    setvbuf(stdout, _IOLBF, (char *) NULL, 0);
#else
    setvbuf(stderr, (char *) NULL, _IONBF, 0);
    setvbuf(stdout, (char *) NULL, _IOLBF, 0);
#endif
}
