static char* exec_vp(char **args, int redirect_stderr, int exec_timeout_sec, int *status)
{
     
    static const char *const env_vec[] = {
        "LANG",
        "LC_ALL",
        "LC_COLLATE",
        "LC_CTYPE",
        "LC_MESSAGES",
        "LC_MONETARY",
        "LC_NUMERIC",
        "LC_TIME",
         
        "TERM",
        NULL
    };

    int flags = EXECFLG_INPUT_NUL | EXECFLG_OUTPUT | EXECFLG_SETSID | EXECFLG_QUIET;
    if (redirect_stderr)
        flags |= EXECFLG_ERR2OUT;
    VERB1 flags &= ~EXECFLG_QUIET;

    int pipeout[2];
    pid_t child = fork_execv_on_steroids(flags, args, pipeout, (char**)env_vec,   NULL,   0);

     
    ndelay_on(pipeout[0]);
    int t = time(NULL);  
    int endtime = t + exec_timeout_sec;
    struct strbuf *buf_out = strbuf_new();
    while (1)
    {
        int timeout = endtime - t;
        if (timeout < 0)
        {
            kill(child, SIGKILL);
            strbuf_append_strf(buf_out, "\n"
                        "Timeout exceeded: %u seconds, killing %s.\n"
                        "Looks like gdb hung while generating backtrace.\n"
                        "This may be a bug in gdb. Consider submitting a bug report to gdb developers.\n"
                        "Please attach coredump from this crash to the bug report if you do.\n",
                        exec_timeout_sec, args[0]
            );
            break;
        }

         
        struct pollfd pfd;
        pfd.fd = pipeout[0];
        pfd.events = POLLIN;
        poll(&pfd, 1, timeout * 1000);

        char buff[1024];
        int r = read(pipeout[0], buff, sizeof(buff) - 1);
        if (r <= 0)
        {
             
            if (r < 0 && errno == EAGAIN)
                goto next;
            break;
        }
        buff[r] = '\0';
        strbuf_append_str(buf_out, buff);
 next:
        t = time(NULL);
    }
    close(pipeout[0]);

     
    safe_waitpid(child, status, 0);

    return strbuf_free_nobuf(buf_out);
}
