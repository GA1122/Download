int service_main(int argc, char **argv, char **envp __attribute__((unused)))
#else
int service_main(int argc __attribute__((unused)),
                 char **argv __attribute__((unused)),
                 char **envp __attribute__((unused)))
#endif
{
    sasl_security_properties_t *secprops = NULL;
    const char *localip, *remoteip;
    struct mboxevent *mboxevent = NULL;
    struct io_count *io_count_start = NULL;
    struct io_count *io_count_stop = NULL;

    if (config_iolog) {
        io_count_start = xmalloc (sizeof (struct io_count));
        io_count_stop = xmalloc (sizeof (struct io_count));
        read_io_count(io_count_start);
    }

    session_new_id();

    signals_poll();

#ifdef ID_SAVE_CMDLINE
     
    id_getcmdline(argc, argv);
#endif

    sync_log_init();

    imapd_in = prot_new(0, 0);
    imapd_out = prot_new(1, 1);
    protgroup_insert(protin, imapd_in);

     
    imapd_clienthost = get_clienthost(0, &localip, &remoteip);

     
    if (sasl_server_new("imap", config_servername,
                        NULL, NULL, NULL, NULL, 0,
                        &imapd_saslconn) != SASL_OK) {
        fatal("SASL failed initializing: sasl_server_new()", EC_TEMPFAIL);
    }

    secprops = mysasl_secprops(0);
    if (sasl_setprop(imapd_saslconn, SASL_SEC_PROPS, secprops) != SASL_OK)
        fatal("Failed to set SASL property", EC_TEMPFAIL);
    if (sasl_setprop(imapd_saslconn, SASL_SSF_EXTERNAL, &extprops_ssf) != SASL_OK)
        fatal("Failed to set SASL property", EC_TEMPFAIL);

    if (localip && remoteip) {
        sasl_setprop(imapd_saslconn, SASL_IPREMOTEPORT, remoteip);
        saslprops.ipremoteport = xstrdup(remoteip);
        sasl_setprop(imapd_saslconn, SASL_IPLOCALPORT, localip);
        saslprops.iplocalport = xstrdup(localip);
    }

    imapd_tls_required = config_getswitch(IMAPOPT_TLS_REQUIRED);

    proc_register(config_ident, imapd_clienthost, NULL, NULL, NULL);

     
    imapd_timeout = config_getint(IMAPOPT_TIMEOUT);
    if (imapd_timeout < 30) imapd_timeout = 30;
    imapd_timeout *= 60;
    prot_settimeout(imapd_in, imapd_timeout);
    prot_setflushonread(imapd_in, imapd_out);

     
    if (imaps == 1) cmd_starttls(NULL, 1);

    snmp_increment(TOTAL_CONNECTIONS, 1);
    snmp_increment(ACTIVE_CONNECTIONS, 1);

     
    mboxname_init_namespace(&imapd_namespace,  1);
    mboxevent_setnamespace(&imapd_namespace);

    cmdloop();

     
    prot_flush(imapd_out);
    snmp_increment(ACTIVE_CONNECTIONS, -1);

     
    if ((mboxevent = mboxevent_new(EVENT_LOGOUT))) {
        mboxevent_set_access(mboxevent, saslprops.iplocalport,
                             saslprops.ipremoteport, imapd_userid, NULL, 1);

        mboxevent_notify(&mboxevent);
        mboxevent_free(&mboxevent);
    }

     
    imapd_reset();

    if (config_iolog) {
        read_io_count(io_count_stop);
        syslog(LOG_INFO,
               "IMAP session stats : I/O read : %d bytes : I/O write : %d bytes",
                io_count_stop->io_read_count - io_count_start->io_read_count,
                io_count_stop->io_write_count - io_count_start->io_write_count);
        free (io_count_start);
        free (io_count_stop);
    }

    return 0;
}
