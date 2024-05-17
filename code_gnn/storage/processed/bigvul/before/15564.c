main(int ac, char **av)
{
	int i, r, opt, exit_status, use_syslog, config_test = 0;
	char *p, *cp, *line, *argv0, buf[PATH_MAX], *host_arg, *logfile;
	char thishost[NI_MAXHOST], shorthost[NI_MAXHOST], portstr[NI_MAXSERV];
	char cname[NI_MAXHOST], uidstr[32], *conn_hash_hex;
	struct stat st;
	struct passwd *pw;
	int timeout_ms;
	extern int optind, optreset;
	extern char *optarg;
	struct Forward fwd;
	struct addrinfo *addrs = NULL;
	struct ssh_digest_ctx *md;
	u_char conn_hash[SSH_DIGEST_MAX_LENGTH];

	 
	sanitise_stdfd();

	__progname = ssh_get_progname(av[0]);

#ifndef HAVE_SETPROCTITLE
	 
	 
	saved_av = xcalloc(ac + 1, sizeof(*saved_av));
	for (i = 0; i < ac; i++)
		saved_av[i] = xstrdup(av[i]);
	saved_av[i] = NULL;
	compat_init_setproctitle(ac, av);
	av = saved_av;
#endif

	 
	closefrom(STDERR_FILENO + 1);

	 
	original_real_uid = getuid();
	original_effective_uid = geteuid();

	 
	PRIV_END;

#ifdef HAVE_SETRLIMIT
	 
	if (original_real_uid != original_effective_uid) {
		struct rlimit rlim;
		rlim.rlim_cur = rlim.rlim_max = 0;
		if (setrlimit(RLIMIT_CORE, &rlim) < 0)
			fatal("setrlimit failed: %.100s", strerror(errno));
	}
#endif
	 
	pw = getpwuid(original_real_uid);
	if (!pw) {
		logit("No user exists for uid %lu", (u_long)original_real_uid);
		exit(255);
	}
	 
	pw = pwcopy(pw);

	 
	umask(022);

	 
	initialize_options(&options);

	 
	host = NULL;
	use_syslog = 0;
	logfile = NULL;
	argv0 = av[0];

 again:
	while ((opt = getopt(ac, av, "1246ab:c:e:fgi:kl:m:no:p:qstvx"
	    "ACD:E:F:GI:KL:MNO:PQ:R:S:TVw:W:XYy")) != -1) {
		switch (opt) {
		case '1':
			options.protocol = SSH_PROTO_1;
			break;
		case '2':
			options.protocol = SSH_PROTO_2;
			break;
		case '4':
			options.address_family = AF_INET;
			break;
		case '6':
			options.address_family = AF_INET6;
			break;
		case 'n':
			stdin_null_flag = 1;
			break;
		case 'f':
			fork_after_authentication_flag = 1;
			stdin_null_flag = 1;
			break;
		case 'x':
			options.forward_x11 = 0;
			break;
		case 'X':
			options.forward_x11 = 1;
			break;
		case 'y':
			use_syslog = 1;
			break;
		case 'E':
			logfile = optarg;
			break;
		case 'G':
			config_test = 1;
			break;
		case 'Y':
			options.forward_x11 = 1;
			options.forward_x11_trusted = 1;
			break;
		case 'g':
			options.fwd_opts.gateway_ports = 1;
			break;
		case 'O':
			if (stdio_forward_host != NULL)
				fatal("Cannot specify multiplexing "
				    "command with -W");
			else if (muxclient_command != 0)
				fatal("Multiplexing command already specified");
			if (strcmp(optarg, "check") == 0)
				muxclient_command = SSHMUX_COMMAND_ALIVE_CHECK;
			else if (strcmp(optarg, "forward") == 0)
				muxclient_command = SSHMUX_COMMAND_FORWARD;
			else if (strcmp(optarg, "exit") == 0)
				muxclient_command = SSHMUX_COMMAND_TERMINATE;
			else if (strcmp(optarg, "stop") == 0)
				muxclient_command = SSHMUX_COMMAND_STOP;
			else if (strcmp(optarg, "cancel") == 0)
				muxclient_command = SSHMUX_COMMAND_CANCEL_FWD;
			else
				fatal("Invalid multiplex command.");
			break;
		case 'P':	 
			options.use_privileged_port = 0;
			break;
		case 'Q':
			cp = NULL;
			if (strcmp(optarg, "cipher") == 0)
				cp = cipher_alg_list('\n', 0);
			else if (strcmp(optarg, "cipher-auth") == 0)
				cp = cipher_alg_list('\n', 1);
			else if (strcmp(optarg, "mac") == 0)
				cp = mac_alg_list('\n');
			else if (strcmp(optarg, "kex") == 0)
				cp = kex_alg_list('\n');
			else if (strcmp(optarg, "key") == 0)
				cp = key_alg_list(0, 0);
			else if (strcmp(optarg, "key-cert") == 0)
				cp = key_alg_list(1, 0);
			else if (strcmp(optarg, "key-plain") == 0)
				cp = key_alg_list(0, 1);
			else if (strcmp(optarg, "protocol-version") == 0) {
#ifdef WITH_SSH1
				cp = xstrdup("1\n2");
#else
				cp = xstrdup("2");
#endif
			}
			if (cp == NULL)
				fatal("Unsupported query \"%s\"", optarg);
			printf("%s\n", cp);
			free(cp);
			exit(0);
			break;
		case 'a':
			options.forward_agent = 0;
			break;
		case 'A':
			options.forward_agent = 1;
			break;
		case 'k':
			options.gss_deleg_creds = 0;
			break;
		case 'K':
			options.gss_authentication = 1;
			options.gss_deleg_creds = 1;
			break;
		case 'i':
			p = tilde_expand_filename(optarg, original_real_uid);
			if (stat(p, &st) < 0)
				fprintf(stderr, "Warning: Identity file %s "
				    "not accessible: %s.\n", p,
				    strerror(errno));
			else
				add_identity_file(&options, NULL, p, 1);
			free(p);
			break;
		case 'I':
#ifdef ENABLE_PKCS11
			free(options.pkcs11_provider);
			options.pkcs11_provider = xstrdup(optarg);
#else
			fprintf(stderr, "no support for PKCS#11.\n");
#endif
			break;
		case 't':
			if (options.request_tty == REQUEST_TTY_YES)
				options.request_tty = REQUEST_TTY_FORCE;
			else
				options.request_tty = REQUEST_TTY_YES;
			break;
		case 'v':
			if (debug_flag == 0) {
				debug_flag = 1;
				options.log_level = SYSLOG_LEVEL_DEBUG1;
			} else {
				if (options.log_level < SYSLOG_LEVEL_DEBUG3)
					options.log_level++;
			}
			break;
		case 'V':
			fprintf(stderr, "%s, %s\n",
			    SSH_RELEASE,
#ifdef WITH_OPENSSL
			    SSLeay_version(SSLEAY_VERSION)
#else
			    "without OpenSSL"
#endif
			);
			if (opt == 'V')
				exit(0);
			break;
		case 'w':
			if (options.tun_open == -1)
				options.tun_open = SSH_TUNMODE_DEFAULT;
			options.tun_local = a2tun(optarg, &options.tun_remote);
			if (options.tun_local == SSH_TUNID_ERR) {
				fprintf(stderr,
				    "Bad tun device '%s'\n", optarg);
				exit(255);
			}
			break;
		case 'W':
			if (stdio_forward_host != NULL)
				fatal("stdio forward already specified");
			if (muxclient_command != 0)
				fatal("Cannot specify stdio forward with -O");
			if (parse_forward(&fwd, optarg, 1, 0)) {
				stdio_forward_host = fwd.listen_host;
				stdio_forward_port = fwd.listen_port;
				free(fwd.connect_host);
			} else {
				fprintf(stderr,
				    "Bad stdio forwarding specification '%s'\n",
				    optarg);
				exit(255);
			}
			options.request_tty = REQUEST_TTY_NO;
			no_shell_flag = 1;
			options.clear_forwardings = 1;
			options.exit_on_forward_failure = 1;
			break;
		case 'q':
			options.log_level = SYSLOG_LEVEL_QUIET;
			break;
		case 'e':
			if (optarg[0] == '^' && optarg[2] == 0 &&
			    (u_char) optarg[1] >= 64 &&
			    (u_char) optarg[1] < 128)
				options.escape_char = (u_char) optarg[1] & 31;
			else if (strlen(optarg) == 1)
				options.escape_char = (u_char) optarg[0];
			else if (strcmp(optarg, "none") == 0)
				options.escape_char = SSH_ESCAPECHAR_NONE;
			else {
				fprintf(stderr, "Bad escape character '%s'.\n",
				    optarg);
				exit(255);
			}
			break;
		case 'c':
			if (ciphers_valid(*optarg == '+' ?
			    optarg + 1 : optarg)) {
				 
				free(options.ciphers);
				options.ciphers = xstrdup(optarg);
				options.cipher = SSH_CIPHER_INVALID;
				break;
			}
			 
			options.cipher = cipher_number(optarg);
			if (options.cipher == -1) {
				fprintf(stderr, "Unknown cipher type '%s'\n",
				    optarg);
				exit(255);
			}
			if (options.cipher == SSH_CIPHER_3DES)
				options.ciphers = xstrdup("3des-cbc");
			else if (options.cipher == SSH_CIPHER_BLOWFISH)
				options.ciphers = xstrdup("blowfish-cbc");
			else
				options.ciphers = xstrdup(KEX_CLIENT_ENCRYPT);
			break;
		case 'm':
			if (mac_valid(optarg)) {
				free(options.macs);
				options.macs = xstrdup(optarg);
			} else {
				fprintf(stderr, "Unknown mac type '%s'\n",
				    optarg);
				exit(255);
			}
			break;
		case 'M':
			if (options.control_master == SSHCTL_MASTER_YES)
				options.control_master = SSHCTL_MASTER_ASK;
			else
				options.control_master = SSHCTL_MASTER_YES;
			break;
		case 'p':
			options.port = a2port(optarg);
			if (options.port <= 0) {
				fprintf(stderr, "Bad port '%s'\n", optarg);
				exit(255);
			}
			break;
		case 'l':
			options.user = optarg;
			break;

		case 'L':
			if (parse_forward(&fwd, optarg, 0, 0))
				add_local_forward(&options, &fwd);
			else {
				fprintf(stderr,
				    "Bad local forwarding specification '%s'\n",
				    optarg);
				exit(255);
			}
			break;

		case 'R':
			if (parse_forward(&fwd, optarg, 0, 1)) {
				add_remote_forward(&options, &fwd);
			} else {
				fprintf(stderr,
				    "Bad remote forwarding specification "
				    "'%s'\n", optarg);
				exit(255);
			}
			break;

		case 'D':
			if (parse_forward(&fwd, optarg, 1, 0)) {
				add_local_forward(&options, &fwd);
			} else {
				fprintf(stderr,
				    "Bad dynamic forwarding specification "
				    "'%s'\n", optarg);
				exit(255);
			}
			break;

		case 'C':
			options.compression = 1;
			break;
		case 'N':
			no_shell_flag = 1;
			options.request_tty = REQUEST_TTY_NO;
			break;
		case 'T':
			options.request_tty = REQUEST_TTY_NO;
			break;
		case 'o':
			line = xstrdup(optarg);
			if (process_config_line(&options, pw,
			    host ? host : "", host ? host : "", line,
			    "command-line", 0, NULL, SSHCONF_USERCONF) != 0)
				exit(255);
			free(line);
			break;
		case 's':
			subsystem_flag = 1;
			break;
		case 'S':
			free(options.control_path);
			options.control_path = xstrdup(optarg);
			break;
		case 'b':
			options.bind_address = optarg;
			break;
		case 'F':
			config = optarg;
			break;
		default:
			usage();
		}
	}

	ac -= optind;
	av += optind;

	if (ac > 0 && !host) {
		if (strrchr(*av, '@')) {
			p = xstrdup(*av);
			cp = strrchr(p, '@');
			if (cp == NULL || cp == p)
				usage();
			options.user = p;
			*cp = '\0';
			host = xstrdup(++cp);
		} else
			host = xstrdup(*av);
		if (ac > 1) {
			optind = optreset = 1;
			goto again;
		}
		ac--, av++;
	}

	 
	if (!host)
		usage();

	host_arg = xstrdup(host);

#ifdef WITH_OPENSSL
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();
#endif

	 
	buffer_init(&command);

	 
	if (!ac) {
		 
		if (subsystem_flag) {
			fprintf(stderr,
			    "You must specify a subsystem to invoke.\n");
			usage();
		}
	} else {
		 
		for (i = 0; i < ac; i++) {
			if (i)
				buffer_append(&command, " ", 1);
			buffer_append(&command, av[i], strlen(av[i]));
		}
	}

	 
	if (fork_after_authentication_flag && buffer_len(&command) == 0 &&
	    !no_shell_flag)
		fatal("Cannot fork into background without a command "
		    "to execute.");

	 
	if (use_syslog && logfile != NULL)
		fatal("Can't specify both -y and -E");
	if (logfile != NULL)
		log_redirect_stderr_to(logfile);
	log_init(argv0,
	    options.log_level == -1 ? SYSLOG_LEVEL_INFO : options.log_level,
	    SYSLOG_FACILITY_USER, !use_syslog);

	if (debug_flag)
		logit("%s, %s", SSH_RELEASE,
#ifdef WITH_OPENSSL
		    SSLeay_version(SSLEAY_VERSION)
#else
		    "without OpenSSL"
#endif
		);

	 
	process_config_files(host_arg, pw, 0);

	 
	fill_default_options_for_canonicalization(&options);

	 
	if (options.hostname != NULL) {
		 
		cp = percent_expand(options.hostname,
		    "h", host, (char *)NULL);
		free(host);
		host = cp;
		free(options.hostname);
		options.hostname = xstrdup(host);
	}

	 
	lowercase(host);
	if (options.canonicalize_hostname != SSH_CANONICALISE_NO)
		addrs = resolve_canonicalize(&host, options.port);

	 
	if (addrs == NULL && options.num_permitted_cnames != 0 &&
	    (option_clear_or_none(options.proxy_command) ||
            options.canonicalize_hostname == SSH_CANONICALISE_ALWAYS)) {
		if ((addrs = resolve_host(host, options.port,
		    option_clear_or_none(options.proxy_command),
		    cname, sizeof(cname))) == NULL) {
			 
			if (option_clear_or_none(options.proxy_command))
				cleanup_exit(255);  
		} else
			check_follow_cname(&host, cname);
	}

	 
	if (options.canonicalize_hostname != 0) {
		debug("Re-reading configuration after hostname "
		    "canonicalisation");
		free(options.hostname);
		options.hostname = xstrdup(host);
		process_config_files(host_arg, pw, 1);
		 
		if (addrs != NULL && options.port > 0)
			set_addrinfo_port(addrs, options.port);
	}

	 
	fill_default_options(&options);

	if (options.port == 0)
		options.port = default_ssh_port();
	channel_set_af(options.address_family);

	 
	if (options.host_key_alias != NULL)
		lowercase(options.host_key_alias);
	if (options.proxy_command != NULL &&
	    strcmp(options.proxy_command, "-") == 0 &&
	    options.proxy_use_fdpass)
		fatal("ProxyCommand=- and ProxyUseFDPass are incompatible");
	if (options.control_persist &&
	    options.update_hostkeys == SSH_UPDATE_HOSTKEYS_ASK) {
		debug("UpdateHostKeys=ask is incompatible with ControlPersist; "
		    "disabling");
		options.update_hostkeys = 0;
	}
	if (options.connection_attempts <= 0)
		fatal("Invalid number of ConnectionAttempts");
#ifndef HAVE_CYGWIN
	if (original_effective_uid != 0)
		options.use_privileged_port = 0;
#endif

	 
	log_init(argv0, options.log_level, SYSLOG_FACILITY_USER, !use_syslog);

	if (options.request_tty == REQUEST_TTY_YES ||
	    options.request_tty == REQUEST_TTY_FORCE)
		tty_flag = 1;

	 
	if (buffer_len(&command) == 0)
		tty_flag = options.request_tty != REQUEST_TTY_NO;

	 
	if (options.request_tty == REQUEST_TTY_NO || muxclient_command != 0)
		tty_flag = 0;
	 
	if ((!isatty(fileno(stdin)) || stdin_null_flag) &&
	    options.request_tty != REQUEST_TTY_FORCE) {
		if (tty_flag)
			logit("Pseudo-terminal will not be allocated because "
			    "stdin is not a terminal.");
		tty_flag = 0;
	}

	seed_rng();

	if (options.user == NULL)
		options.user = xstrdup(pw->pw_name);

	if (gethostname(thishost, sizeof(thishost)) == -1)
		fatal("gethostname: %s", strerror(errno));
	strlcpy(shorthost, thishost, sizeof(shorthost));
	shorthost[strcspn(thishost, ".")] = '\0';
	snprintf(portstr, sizeof(portstr), "%d", options.port);
	snprintf(uidstr, sizeof(uidstr), "%d", pw->pw_uid);

	if ((md = ssh_digest_start(SSH_DIGEST_SHA1)) == NULL ||
	    ssh_digest_update(md, thishost, strlen(thishost)) < 0 ||
	    ssh_digest_update(md, host, strlen(host)) < 0 ||
	    ssh_digest_update(md, portstr, strlen(portstr)) < 0 ||
	    ssh_digest_update(md, options.user, strlen(options.user)) < 0 ||
	    ssh_digest_final(md, conn_hash, sizeof(conn_hash)) < 0)
		fatal("%s: mux digest failed", __func__);
	ssh_digest_free(md);
	conn_hash_hex = tohex(conn_hash, ssh_digest_bytes(SSH_DIGEST_SHA1));

	if (options.local_command != NULL) {
		debug3("expanding LocalCommand: %s", options.local_command);
		cp = options.local_command;
		options.local_command = percent_expand(cp,
		    "C", conn_hash_hex,
		    "L", shorthost,
		    "d", pw->pw_dir,
		    "h", host,
		    "l", thishost,
		    "n", host_arg,
		    "p", portstr,
		    "r", options.user,
		    "u", pw->pw_name,
		    (char *)NULL);
		debug3("expanded LocalCommand: %s", options.local_command);
		free(cp);
	}

	if (options.control_path != NULL) {
		cp = tilde_expand_filename(options.control_path,
		    original_real_uid);
		free(options.control_path);
		options.control_path = percent_expand(cp,
		    "C", conn_hash_hex,
		    "L", shorthost,
		    "h", host,
		    "l", thishost,
		    "n", host_arg,
		    "p", portstr,
		    "r", options.user,
		    "u", pw->pw_name,
		    "i", uidstr,
		    (char *)NULL);
		free(cp);
	}
	free(conn_hash_hex);

	if (config_test) {
		dump_client_config(&options, host);
		exit(0);
	}

	if (muxclient_command != 0 && options.control_path == NULL)
		fatal("No ControlPath specified for \"-O\" command");
	if (options.control_path != NULL)
		muxclient(options.control_path);

	 
	if (addrs == NULL && options.proxy_command == NULL) {
		debug2("resolving \"%s\" port %d", host, options.port);
		if ((addrs = resolve_host(host, options.port, 1,
		    cname, sizeof(cname))) == NULL)
			cleanup_exit(255);  
	}

	timeout_ms = options.connection_timeout * 1000;

	 
	if (ssh_connect(host, addrs, &hostaddr, options.port,
	    options.address_family, options.connection_attempts,
	    &timeout_ms, options.tcp_keep_alive,
	    options.use_privileged_port) != 0)
 		exit(255);

	if (addrs != NULL)
		freeaddrinfo(addrs);

	packet_set_timeout(options.server_alive_interval,
	    options.server_alive_count_max);

	if (timeout_ms > 0)
		debug3("timeout: %d ms remain after connect", timeout_ms);

	 
	sensitive_data.nkeys = 0;
	sensitive_data.keys = NULL;
	sensitive_data.external_keysign = 0;
	if (options.rhosts_rsa_authentication ||
	    options.hostbased_authentication) {
		sensitive_data.nkeys = 9;
		sensitive_data.keys = xcalloc(sensitive_data.nkeys,
		    sizeof(Key));
		for (i = 0; i < sensitive_data.nkeys; i++)
			sensitive_data.keys[i] = NULL;

		PRIV_START;
#if WITH_SSH1
		sensitive_data.keys[0] = key_load_private_type(KEY_RSA1,
		    _PATH_HOST_KEY_FILE, "", NULL, NULL);
#endif
#ifdef OPENSSL_HAS_ECC
		sensitive_data.keys[1] = key_load_private_cert(KEY_ECDSA,
		    _PATH_HOST_ECDSA_KEY_FILE, "", NULL);
#endif
		sensitive_data.keys[2] = key_load_private_cert(KEY_ED25519,
		    _PATH_HOST_ED25519_KEY_FILE, "", NULL);
		sensitive_data.keys[3] = key_load_private_cert(KEY_RSA,
		    _PATH_HOST_RSA_KEY_FILE, "", NULL);
		sensitive_data.keys[4] = key_load_private_cert(KEY_DSA,
		    _PATH_HOST_DSA_KEY_FILE, "", NULL);
#ifdef OPENSSL_HAS_ECC
		sensitive_data.keys[5] = key_load_private_type(KEY_ECDSA,
		    _PATH_HOST_ECDSA_KEY_FILE, "", NULL, NULL);
#endif
		sensitive_data.keys[6] = key_load_private_type(KEY_ED25519,
		    _PATH_HOST_ED25519_KEY_FILE, "", NULL, NULL);
		sensitive_data.keys[7] = key_load_private_type(KEY_RSA,
		    _PATH_HOST_RSA_KEY_FILE, "", NULL, NULL);
		sensitive_data.keys[8] = key_load_private_type(KEY_DSA,
		    _PATH_HOST_DSA_KEY_FILE, "", NULL, NULL);
		PRIV_END;

		if (options.hostbased_authentication == 1 &&
		    sensitive_data.keys[0] == NULL &&
		    sensitive_data.keys[5] == NULL &&
		    sensitive_data.keys[6] == NULL &&
		    sensitive_data.keys[7] == NULL &&
		    sensitive_data.keys[8] == NULL) {
#ifdef OPENSSL_HAS_ECC
			sensitive_data.keys[1] = key_load_cert(
			    _PATH_HOST_ECDSA_KEY_FILE);
#endif
			sensitive_data.keys[2] = key_load_cert(
			    _PATH_HOST_ED25519_KEY_FILE);
			sensitive_data.keys[3] = key_load_cert(
			    _PATH_HOST_RSA_KEY_FILE);
			sensitive_data.keys[4] = key_load_cert(
			    _PATH_HOST_DSA_KEY_FILE);
#ifdef OPENSSL_HAS_ECC
			sensitive_data.keys[5] = key_load_public(
			    _PATH_HOST_ECDSA_KEY_FILE, NULL);
#endif
			sensitive_data.keys[6] = key_load_public(
			    _PATH_HOST_ED25519_KEY_FILE, NULL);
			sensitive_data.keys[7] = key_load_public(
			    _PATH_HOST_RSA_KEY_FILE, NULL);
			sensitive_data.keys[8] = key_load_public(
			    _PATH_HOST_DSA_KEY_FILE, NULL);
			sensitive_data.external_keysign = 1;
		}
	}
	 
	if (original_effective_uid == 0) {
		PRIV_START;
		permanently_set_uid(pw);
	}

	 
	if (config == NULL) {
		r = snprintf(buf, sizeof buf, "%s%s%s", pw->pw_dir,
		    strcmp(pw->pw_dir, "/") ? "/" : "", _PATH_SSH_USER_DIR);
		if (r > 0 && (size_t)r < sizeof(buf) && stat(buf, &st) < 0) {
#ifdef WITH_SELINUX
			ssh_selinux_setfscreatecon(buf);
#endif
			if (mkdir(buf, 0700) < 0)
				error("Could not create directory '%.200s'.",
				    buf);
#ifdef WITH_SELINUX
			ssh_selinux_setfscreatecon(NULL);
#endif
		}
	}
	 
	load_public_identity_files();

	 
	tilde_expand_paths(options.system_hostfiles,
	    options.num_system_hostfiles);
	tilde_expand_paths(options.user_hostfiles, options.num_user_hostfiles);

	signal(SIGPIPE, SIG_IGN);  
	signal(SIGCHLD, main_sigchld_handler);

	 
	ssh_login(&sensitive_data, host, (struct sockaddr *)&hostaddr,
	    options.port, pw, timeout_ms);

	if (packet_connection_is_on_socket()) {
		verbose("Authenticated to %s ([%s]:%d).", host,
		    get_remote_ipaddr(), get_remote_port());
	} else {
		verbose("Authenticated to %s (via proxy).", host);
	}

	 
	if (sensitive_data.nkeys != 0) {
		for (i = 0; i < sensitive_data.nkeys; i++) {
			if (sensitive_data.keys[i] != NULL) {
				 
				debug3("clear hostkey %d", i);
				key_free(sensitive_data.keys[i]);
				sensitive_data.keys[i] = NULL;
			}
		}
		free(sensitive_data.keys);
	}
	for (i = 0; i < options.num_identity_files; i++) {
		free(options.identity_files[i]);
		options.identity_files[i] = NULL;
		if (options.identity_keys[i]) {
			key_free(options.identity_keys[i]);
			options.identity_keys[i] = NULL;
		}
	}
	for (i = 0; i < options.num_certificate_files; i++) {
		free(options.certificate_files[i]);
		options.certificate_files[i] = NULL;
	}

	exit_status = compat20 ? ssh_session2() : ssh_session();
	packet_close();

	if (options.control_path != NULL && muxserver_sock != -1)
		unlink(options.control_path);

	 
	ssh_kill_proxy_command();

	return exit_status;
}
