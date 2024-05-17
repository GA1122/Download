main(int argc, char **argv)
{
    int flag;
    int argerr = 0;
    int exit_code = 0;
    int option_index = 0;

    pid_file = strdup("/tmp/ClusterMon.pid");
    crm_log_cli_init("crm_mon");
    crm_set_options(NULL, "mode [options]", long_options,
                    "Provides a summary of cluster's current state."
                    "\n\nOutputs varying levels of detail in a number of different formats.\n");

#ifndef ON_DARWIN
     
    signal(SIGCLD, SIG_IGN);
#endif

    if (strcmp(crm_system_name, "crm_mon.cgi") == 0) {
        web_cgi = TRUE;
        one_shot = TRUE;
    }

    while (1) {
        flag = crm_get_option(argc, argv, &option_index);
        if (flag == -1)
            break;

        switch (flag) {
            case 'V':
                crm_bump_log_level(argc, argv);
                break;
            case 'Q':
                print_last_updated = FALSE;
                print_last_change = FALSE;
                break;
            case 'i':
                reconnect_msec = crm_get_msec(optarg);
                break;
            case 'n':
                group_by_node = TRUE;
                break;
            case 'r':
                inactive_resources = TRUE;
                break;
            case 'W':
                watch_fencing = TRUE;
                break;
            case 'd':
                daemonize = TRUE;
                break;
            case 't':
                print_timing = TRUE;
                print_operations = TRUE;
                break;
            case 'o':
                print_operations = TRUE;
                break;
            case 'f':
                print_failcount = TRUE;
                break;
            case 'A':
                print_nodes_attr = TRUE;
                break;
            case 'c':
                print_tickets = TRUE;
                break;
            case 'p':
                free(pid_file);
                pid_file = strdup(optarg);
                break;
            case 'x':
                xml_file = strdup(optarg);
                one_shot = TRUE;
                break;
            case 'h':
                as_html_file = strdup(optarg);
                break;
            case 'X':
                as_xml = TRUE;
                one_shot = TRUE;
                break;
            case 'w':
                web_cgi = TRUE;
                one_shot = TRUE;
                break;
            case 's':
                simple_status = TRUE;
                one_shot = TRUE;
                break;
            case 'S':
                snmp_target = optarg;
                break;
            case 'T':
                crm_mail_to = optarg;
                break;
            case 'F':
                crm_mail_from = optarg;
                break;
            case 'H':
                crm_mail_host = optarg;
                break;
            case 'P':
                crm_mail_prefix = optarg;
                break;
            case 'E':
                external_agent = optarg;
                break;
            case 'e':
                external_recipient = optarg;
                break;
            case '1':
                one_shot = TRUE;
                break;
            case 'N':
                as_console = FALSE;
                break;
            case 'C':
                snmp_community = optarg;
                break;
            case '$':
            case '?':
                crm_help(flag, EX_OK);
                break;
            default:
                printf("Argument code 0%o (%c) is not (?yet?) supported\n", flag, flag);
                ++argerr;
                break;
        }
    }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
    if (argerr) {
        crm_help('?', EX_USAGE);
    }

    if (one_shot) {
        as_console = FALSE;

    } else if (daemonize) {
        as_console = FALSE;
        crm_enable_stderr(FALSE);

        if (!as_html_file && !snmp_target && !crm_mail_to && !external_agent && !as_xml) {
            printf
                ("Looks like you forgot to specify one or more of: --as-html, --as-xml, --mail-to, --snmp-target, --external-agent\n");
            crm_help('?', EX_USAGE);
        }

        crm_make_daemon(crm_system_name, TRUE, pid_file);

    } else if (as_console) {
#if CURSES_ENABLED
        initscr();
        cbreak();
        noecho();
        crm_enable_stderr(FALSE);
#else
        one_shot = TRUE;
        as_console = FALSE;
        printf("Defaulting to one-shot mode\n");
        printf("You need to have curses available at compile time to enable console mode\n");
#endif
    }

    crm_info("Starting %s", crm_system_name);
    if (xml_file != NULL) {
        current_cib = filename2xml(xml_file);
        mon_refresh_display(NULL);
        return exit_code;
    }

    if (current_cib == NULL) {
        cib = cib_new();
        if (!one_shot) {
            print_as("Attempting connection to the cluster...");
        }

        do {
            exit_code = cib_connect(!one_shot);

            if (one_shot) {
                break;

            } else if (exit_code != pcmk_ok) {
                print_dot();
                sleep(reconnect_msec / 1000);
            }

        } while (exit_code == -ENOTCONN);

        if (exit_code != pcmk_ok) {
            print_as("\nConnection to cluster failed: %s\n", pcmk_strerror(exit_code));
            if (as_console) {
                sleep(2);
            }
            clean_up(-exit_code);
        }
    }

    if (one_shot) {
        return exit_code;
    }

    mainloop = g_main_new(FALSE);

    mainloop_add_signal(SIGTERM, mon_shutdown);
    mainloop_add_signal(SIGINT, mon_shutdown);
#if CURSES_ENABLED
    if (as_console) {
        ncurses_winch_handler = signal(SIGWINCH, mon_winresize);
        if (ncurses_winch_handler == SIG_DFL ||
            ncurses_winch_handler == SIG_IGN || ncurses_winch_handler == SIG_ERR)
            ncurses_winch_handler = NULL;
    }
#endif
    refresh_trigger = mainloop_add_trigger(G_PRIORITY_LOW, mon_refresh_display, NULL);

    g_main_run(mainloop);
    g_main_destroy(mainloop);

    crm_info("Exiting %s", crm_system_name);

    clean_up(0);
    return 0;                    
}