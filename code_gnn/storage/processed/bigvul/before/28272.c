create_workers(verto_ctx *ctx, int num)
{
    krb5_error_code retval;
    int i, status;
    pid_t pid, *pids;
#ifdef POSIX_SIGNALS
    struct sigaction s_action;
#endif  

     
#ifdef POSIX_SIGNALS
    (void) sigemptyset(&s_action.sa_mask);
    s_action.sa_flags = 0;
    s_action.sa_handler = on_monitor_signal;
    (void) sigaction(SIGINT, &s_action, (struct sigaction *) NULL);
    (void) sigaction(SIGTERM, &s_action, (struct sigaction *) NULL);
    (void) sigaction(SIGQUIT, &s_action, (struct sigaction *) NULL);
    s_action.sa_handler = on_monitor_sighup;
    (void) sigaction(SIGHUP, &s_action, (struct sigaction *) NULL);
#else   
    signal(SIGINT, on_monitor_signal);
    signal(SIGTERM, on_monitor_signal);
    signal(SIGQUIT, on_monitor_signal);
    signal(SIGHUP, on_monitor_sighup);
#endif  

     
    krb5_klog_syslog(LOG_INFO, _("creating %d worker processes"), num);
    pids = calloc(num, sizeof(pid_t));
    if (pids == NULL)
        return ENOMEM;
    for (i = 0; i < num; i++) {
        pid = fork();
        if (pid == 0) {
            if (!verto_reinitialize(ctx)) {
                krb5_klog_syslog(LOG_ERR,
                                 _("Unable to reinitialize main loop"));
                return ENOMEM;
            }
            retval = loop_setup_signals(ctx, NULL, reset_for_hangup);
            if (retval) {
                krb5_klog_syslog(LOG_ERR, _("Unable to initialize signal "
                                            "handlers in pid %d"), pid);
                return retval;
            }

             
            if (signal_received)
                exit(0);

             
            free(pids);
            return 0;
        }
        if (pid == -1) {
             
            status = errno;
            terminate_workers(pids, i);
            free(pids);
            return status;
        }
        pids[i] = pid;
    }

     
    loop_free(ctx);

     
    while (!signal_received) {
         
        pid = wait(&status);
        if (pid >= 0) {
            krb5_klog_syslog(LOG_ERR, _("worker %ld exited with status %d"),
                             (long) pid, status);

             
            for (i = 0; i < num; i++) {
                if (pids[i] == pid)
                    pids[i] = -1;
            }

             
            break;
        }

         
        if (sighup_received) {
            sighup_received = 0;
            for (i = 0; i < num; i++) {
                if (pids[i] != -1)
                    kill(pids[i], SIGHUP);
            }
        }
    }
    if (signal_received)
        krb5_klog_syslog(LOG_INFO, _("signal %d received in supervisor"),
                         signal_received);

    terminate_workers(pids, num);
    free(pids);
    exit(0);
}