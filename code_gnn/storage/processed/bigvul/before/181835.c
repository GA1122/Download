 main(int ac, char **av)
 {
 	int c_flag = 0, d_flag = 0, D_flag = 0, k_flag = 0, s_flag = 0;
 	int sock, fd, ch, result, saved_errno;
 	u_int nalloc;
 	char *shell, *format, *pidstr, *agentsocket = NULL;
 	fd_set *readsetp = NULL, *writesetp = NULL;
 	struct rlimit rlim;
 	extern int optind;
 	extern char *optarg;
 	pid_t pid;
 	char pidstrbuf[1 + 3 * sizeof pid];
 	struct timeval *tvp = NULL;
 	size_t len;
 	mode_t prev_mask;
 
 	ssh_malloc_init();	 
 	 
 	sanitise_stdfd();
 
 	 
 	setegid(getgid());
 	setgid(getgid());
 
 #ifdef WITH_OPENSSL
  	OpenSSL_add_all_algorithms();
  #endif
  
	while ((ch = getopt(ac, av, "cDdksE:a:t:")) != -1) {
// 	while ((ch = getopt(ac, av, "cDdksE:a:P:t:")) != -1) {
  		switch (ch) {
  		case 'E':
  			fingerprint_hash = ssh_digest_alg_by_name(optarg);
 			if (fingerprint_hash == -1)
 				fatal("Invalid hash algorithm \"%s\"", optarg);
 			break;
 		case 'c':
 			if (s_flag)
 				usage();
 			c_flag++;
 			break;
  		case 'k':
  			k_flag++;
  			break;
// 		case 'P':
// 			if (pkcs11_whitelist != NULL)
// 				fatal("-P option already specified");
// 			pkcs11_whitelist = xstrdup(optarg);
// 			break;
  		case 's':
  			if (c_flag)
  				usage();
 			s_flag++;
 			break;
 		case 'd':
 			if (d_flag || D_flag)
 				usage();
 			d_flag++;
 			break;
 		case 'D':
 			if (d_flag || D_flag)
 				usage();
 			D_flag++;
 			break;
 		case 'a':
 			agentsocket = optarg;
 			break;
 		case 't':
 			if ((lifetime = convtime(optarg)) == -1) {
 				fprintf(stderr, "Invalid lifetime\n");
 				usage();
 			}
 			break;
 		default:
 			usage();
 		}
 	}
 	ac -= optind;
 	av += optind;
 
  	if (ac > 0 && (c_flag || k_flag || s_flag || d_flag || D_flag))
  		usage();
  
// 	if (pkcs11_whitelist == NULL)
// 		pkcs11_whitelist = xstrdup(DEFAULT_PKCS11_WHITELIST);
// 
  	if (ac == 0 && !c_flag && !s_flag) {
  		shell = getenv("SHELL");
  		if (shell != NULL && (len = strlen(shell)) > 2 &&
 		    strncmp(shell + len - 3, "csh", 3) == 0)
 			c_flag = 1;
 	}
 	if (k_flag) {
 		const char *errstr = NULL;
 
 		pidstr = getenv(SSH_AGENTPID_ENV_NAME);
 		if (pidstr == NULL) {
 			fprintf(stderr, "%s not set, cannot kill agent\n",
 			    SSH_AGENTPID_ENV_NAME);
 			exit(1);
 		}
 		pid = (int)strtonum(pidstr, 2, INT_MAX, &errstr);
 		if (errstr) {
 			fprintf(stderr,
 			    "%s=\"%s\", which is not a good PID: %s\n",
 			    SSH_AGENTPID_ENV_NAME, pidstr, errstr);
 			exit(1);
 		}
 		if (kill(pid, SIGTERM) == -1) {
 			perror("kill");
 			exit(1);
 		}
 		format = c_flag ? "unsetenv %s;\n" : "unset %s;\n";
 		printf(format, SSH_AUTHSOCKET_ENV_NAME);
 		printf(format, SSH_AGENTPID_ENV_NAME);
 		printf("echo Agent pid %ld killed;\n", (long)pid);
 		exit(0);
 	}
 	parent_pid = getpid();
 
 	if (agentsocket == NULL) {
 		 
 		mktemp_proto(socket_dir, sizeof(socket_dir));
 		if (mkdtemp(socket_dir) == NULL) {
 			perror("mkdtemp: private socket dir");
 			exit(1);
 		}
 		snprintf(socket_name, sizeof socket_name, "%s/agent.%ld", socket_dir,
 		    (long)parent_pid);
 	} else {
 		 
 		socket_dir[0] = '\0';
 		strlcpy(socket_name, agentsocket, sizeof socket_name);
 	}
 
 	 
 	prev_mask = umask(0177);
 	sock = unix_listener(socket_name, SSH_LISTEN_BACKLOG, 0);
 	if (sock < 0) {
 		 
 		*socket_name = '\0';  
 		cleanup_exit(1);
 	}
 	umask(prev_mask);
 
 	 
 	if (D_flag || d_flag) {
 		log_init(__progname,
 		    d_flag ? SYSLOG_LEVEL_DEBUG3 : SYSLOG_LEVEL_INFO,
 		    SYSLOG_FACILITY_AUTH, 1);
 		format = c_flag ? "setenv %s %s;\n" : "%s=%s; export %s;\n";
 		printf(format, SSH_AUTHSOCKET_ENV_NAME, socket_name,
 		    SSH_AUTHSOCKET_ENV_NAME);
 		printf("echo Agent pid %ld;\n", (long)parent_pid);
 		fflush(stdout);
 		goto skip;
 	}
 	pid = fork();
 	if (pid == -1) {
 		perror("fork");
 		cleanup_exit(1);
 	}
 	if (pid != 0) {		 
 		close(sock);
 		snprintf(pidstrbuf, sizeof pidstrbuf, "%ld", (long)pid);
 		if (ac == 0) {
 			format = c_flag ? "setenv %s %s;\n" : "%s=%s; export %s;\n";
 			printf(format, SSH_AUTHSOCKET_ENV_NAME, socket_name,
 			    SSH_AUTHSOCKET_ENV_NAME);
 			printf(format, SSH_AGENTPID_ENV_NAME, pidstrbuf,
 			    SSH_AGENTPID_ENV_NAME);
 			printf("echo Agent pid %ld;\n", (long)pid);
 			exit(0);
 		}
 		if (setenv(SSH_AUTHSOCKET_ENV_NAME, socket_name, 1) == -1 ||
 		    setenv(SSH_AGENTPID_ENV_NAME, pidstrbuf, 1) == -1) {
 			perror("setenv");
 			exit(1);
 		}
 		execvp(av[0], av);
 		perror(av[0]);
 		exit(1);
 	}
 	 
 	log_init(__progname, SYSLOG_LEVEL_INFO, SYSLOG_FACILITY_AUTH, 0);
 
 	if (setsid() == -1) {
 		error("setsid: %s", strerror(errno));
 		cleanup_exit(1);
 	}
 
 	(void)chdir("/");
 	if ((fd = open(_PATH_DEVNULL, O_RDWR, 0)) != -1) {
 		 
 		(void)dup2(fd, STDIN_FILENO);
 		(void)dup2(fd, STDOUT_FILENO);
 		(void)dup2(fd, STDERR_FILENO);
 		if (fd > 2)
 			close(fd);
 	}
 
 	 
 	rlim.rlim_cur = rlim.rlim_max = 0;
 	if (setrlimit(RLIMIT_CORE, &rlim) < 0) {
 		error("setrlimit RLIMIT_CORE: %s", strerror(errno));
 		cleanup_exit(1);
 	}
 
 skip:
 
 	cleanup_pid = getpid();
 
 #ifdef ENABLE_PKCS11
 	pkcs11_init(0);
 #endif
 	new_socket(AUTH_SOCKET, sock);
 	if (ac > 0)
 		parent_alive_interval = 10;
 	idtab_init();
 	signal(SIGPIPE, SIG_IGN);
 	signal(SIGINT, (d_flag | D_flag) ? cleanup_handler : SIG_IGN);
 	signal(SIGHUP, cleanup_handler);
  	signal(SIGTERM, cleanup_handler);
  	nalloc = 0;
  
	if (pledge("stdio cpath unix id proc exec", NULL) == -1)
// 	if (pledge("stdio rpath cpath unix id proc exec", NULL) == -1)
  		fatal("%s: pledge: %s", __progname, strerror(errno));
  
  	while (1) {
 		prepare_select(&readsetp, &writesetp, &max_fd, &nalloc, &tvp);
 		result = select(max_fd + 1, readsetp, writesetp, NULL, tvp);
 		saved_errno = errno;
 		if (parent_alive_interval != 0)
 			check_parent_exists();
 		(void) reaper();	 
 		if (result < 0) {
 			if (saved_errno == EINTR)
 				continue;
 			fatal("select: %s", strerror(saved_errno));
 		} else if (result > 0)
 			after_select(readsetp, writesetp);
 	}
 	 
 }