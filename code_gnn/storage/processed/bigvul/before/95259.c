int service_init(int argc, char **argv, char **envp)
{
    int opt, events;

    if (geteuid() == 0) fatal("must run as the Cyrus user", EC_USAGE);
    setproctitle_init(argc, argv, envp);

     
    signals_set_shutdown(&shut_down);
    signal(SIGPIPE, SIG_IGN);

     
    global_sasl_init(1, 1, mysasl_cb);

     
    mboxlist_init(0);
    mboxlist_open(NULL);

     
    quotadb_init(0);
    quotadb_open(NULL);

     
    denydb_init(0);
    denydb_open(0);

     
    idle_init();

     
    events = mboxevent_init();
    apns_enabled =
      (events & EVENT_APPLEPUSHSERVICE) && config_getstring(IMAPOPT_APS_TOPIC);

    search_attr_init();

     
    snmp_connect();  
    snmp_set_str(SERVER_NAME_VERSION,cyrus_version());

    while ((opt = getopt(argc, argv, "Np:sq")) != EOF) {
        switch (opt) {
        case 's':  
            imaps = 1;
            if (!tls_enabled()) {
                syslog(LOG_ERR, "imaps: required OpenSSL options not present");
                fatal("imaps: required OpenSSL options not present",
                      EC_CONFIG);
            }
            break;
        case 'p':  
            extprops_ssf = atoi(optarg);
            break;
        case 'N':  
            nosaslpasswdcheck = 1;
            break;
        case 'q':  
            ignorequota = 1;
            break;
        default:
            break;
        }
    }

     
    if (config_mupdate_server)
        annotate_init(annotate_fetch_proxy, annotate_store_proxy);
    else
        annotate_init(NULL, NULL);
    annotatemore_open();

    if (config_getswitch(IMAPOPT_STATUSCACHE)) {
        statuscache_open();
    }

     
    protin = protgroup_new(2);

    return 0;
}
