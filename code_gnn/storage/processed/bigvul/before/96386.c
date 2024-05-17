int main(int argc, char **argv)
{
     
    setlocale(LC_ALL, "");
#if ENABLE_NLS
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);
#endif

    abrt_init(argv);

     
    const char *program_usage_string = _(
        "& [options]"
    );
    enum {
        OPT_v = 1 << 0,
        OPT_u = 1 << 1,
        OPT_s = 1 << 2,
        OPT_p = 1 << 3,
    };
     
    struct options program_options[] = {
        OPT__VERBOSE(&g_verbose),
        OPT_INTEGER('u', NULL, &client_uid, _("Use NUM as client uid")),
        OPT_BOOL(   's', NULL, NULL       , _("Log to syslog")),
        OPT_BOOL(   'p', NULL, NULL       , _("Add program names to log")),
        OPT_END()
    };
    unsigned opts = parse_opts(argc, argv, program_options, program_usage_string);

    export_abrt_envvars(opts & OPT_p);

    msg_prefix = xasprintf("%s[%u]", g_progname, getpid());
    if (opts & OPT_s)
    {
        logmode = LOGMODE_JOURNAL;
    }

     
     
    struct sigaction sa;
     
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = dummy_handler;  
    sigaction(SIGALRM, &sa, NULL);
     
    alarm(TIMEOUT);

    if (client_uid == (uid_t)-1L)
    {
         
        struct ucred cr;
        socklen_t crlen = sizeof(cr);
        if (0 != getsockopt(STDIN_FILENO, SOL_SOCKET, SO_PEERCRED, &cr, &crlen))
            perror_msg_and_die("getsockopt(SO_PEERCRED)");
        if (crlen != sizeof(cr))
            error_msg_and_die("%s: bad crlen %d", "getsockopt(SO_PEERCRED)", (int)crlen);
        client_uid = cr.uid;
    }

    load_abrt_conf();

    int r = perform_http_xact();
    if (r == 0)
        r = 200;

    free_abrt_conf_data();

    printf("HTTP/1.1 %u \r\n\r\n", r);

    return (r >= 400);  
}
