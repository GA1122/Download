static pid_t spawn_event_handler_child(const char *dump_dir_name, const char *event_name, int *fdp)
{
    char *args[7];
    args[0] = (char *) LIBEXEC_DIR"/abrt-handle-event";
     
    args[1] = (char *) "-i";
    args[2] = (char *) "-e";
    args[3] = (char *) event_name;
    args[4] = (char *) "--";
    args[5] = (char *) dump_dir_name;
    args[6] = NULL;

    int pipeout[2];
    int flags = EXECFLG_INPUT_NUL | EXECFLG_OUTPUT | EXECFLG_QUIET | EXECFLG_ERR2OUT;
    VERB1 flags &= ~EXECFLG_QUIET;

    char *env_vec[2];
     
    env_vec[0] = xstrdup("REPORT_CLIENT_NONINTERACTIVE=1");
    env_vec[1] = NULL;

    pid_t child = fork_execv_on_steroids(flags, args, pipeout,
                                         env_vec,   NULL,
                                           0);
    if (fdp)
        *fdp = pipeout[0];
    return child;
}