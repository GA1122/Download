static NOINLINE void ntp_init(char **argv)
{
	unsigned opts;
	llist_t *peers;

	srand(getpid());

	if (getuid())
		bb_error_msg_and_die(bb_msg_you_must_be_root);

	 
	G.discipline_jitter = G_precision_sec;
	G.stratum = MAXSTRAT;
	if (BURSTPOLL != 0)
		G.poll_exp = BURSTPOLL;  
	G.last_script_run = G.reftime = G.last_update_recv_time = gettime1900d();  

	 
	peers = NULL;
	opt_complementary = "dd:wn"   
		IF_FEATURE_NTPD_SERVER(":Il");  
	opts = getopt32(argv,
			"nqNx"  
			"wp:*S:"IF_FEATURE_NTPD_SERVER("l")  
			IF_FEATURE_NTPD_SERVER("I:")  
			"d"  
			"46aAbgL",  
			&peers,&G.script_name,
#if ENABLE_FEATURE_NTPD_SERVER
			&G.if_name,
#endif
			&G.verbose);


#if ENABLE_FEATURE_NTPD_SERVER
	G_listen_fd = -1;
	if (opts & OPT_l) {
		G_listen_fd = create_and_bind_dgram_or_die(NULL, 123);
		if (G.if_name) {
			if (setsockopt_bindtodevice(G_listen_fd, G.if_name))
				xfunc_die();
		}
		socket_want_pktinfo(G_listen_fd);
		setsockopt_int(G_listen_fd, IPPROTO_IP, IP_TOS, IPTOS_LOWDELAY);
	}
#endif
	 
	if (opts & OPT_N)
		setpriority(PRIO_PROCESS, 0, -15);

	 
	if (!(opts & OPT_n)) {
		bb_daemonize_or_rexec(DAEMON_DEVNULL_STDIO, argv);
		logmode = LOGMODE_NONE;
	}

	if (peers) {
		while (peers)
			add_peers(llist_pop(&peers));
	}
#if ENABLE_FEATURE_NTPD_CONF
	else {
		parser_t *parser;
		char *token[3];

		parser = config_open("/etc/ntp.conf");
		while (config_read(parser, token, 3, 1, "# \t", PARSE_NORMAL)) {
			if (strcmp(token[0], "server") == 0 && token[1]) {
				add_peers(token[1]);
				continue;
			}
			bb_error_msg("skipping %s:%u: unimplemented command '%s'",
				"/etc/ntp.conf", parser->lineno, token[0]
			);
		}
		config_close(parser);
	}
#endif
	if (G.peer_cnt == 0) {
		if (!(opts & OPT_l))
			bb_show_usage();
		 
		G.stratum = 1;
	}
	 
	if (opts & OPT_q) {
		option_mask32 |= OPT_qq;
		alarm(10);
	}

	bb_signals(0
		| (1 << SIGTERM)
		| (1 << SIGINT)
		| (1 << SIGALRM)
		, record_signo
	);
	bb_signals(0
		| (1 << SIGPIPE)
		| (1 << SIGCHLD)
		, SIG_IGN
	);
}
