do_child(Session *s, const char *command)
{
	struct ssh *ssh = active_state;	 
	extern char **environ;
	char **env;
	char *argv[ARGV_MAX];
	const char *shell, *shell0, *hostname = NULL;
	struct passwd *pw = s->pw;
	int r = 0;

	 
	destroy_sensitive_data();

	 
	if (s->authctxt->force_pwchange) {
		do_setusercontext(pw);
		child_close_fds();
		do_pwchange(s);
		exit(1);
	}

	 
	if (options.use_login && command != NULL)
		options.use_login = 0;

#ifdef _UNICOS
	cray_setup(pw->pw_uid, pw->pw_name, command);
#endif  

	 
	if (!options.use_login) {
#ifdef HAVE_OSF_SIA
		session_setup_sia(pw, s->ttyfd == -1 ? NULL : s->tty);
		if (!check_quietlogin(s, command))
			do_motd();
#else  
		 
		if (!options.use_pam)
			do_nologin(pw);
		do_setusercontext(pw);
		 
		if (!check_quietlogin(s, command))
			display_loginmsg();
#endif  
	}

#ifdef USE_PAM
	if (options.use_pam && !options.use_login && !is_pam_session_open()) {
		debug3("PAM session not opened, exiting");
		display_loginmsg();
		exit(254);
	}
#endif

	 
	shell = (pw->pw_shell[0] == '\0') ? _PATH_BSHELL : pw->pw_shell;

	 
	env = do_setup_env(s, shell);

#ifdef HAVE_LOGIN_CAP
	shell = login_getcapstr(lc, "shell", (char *)shell, (char *)shell);
#endif

	 
	if (options.use_login)
		hostname = session_get_remote_name_or_ip(ssh, utmp_len,
		    options.use_dns);
	 
	child_close_fds();

	 
	environ = env;

#if defined(KRB5) && defined(USE_AFS)
	 

	if (options.kerberos_get_afs_token && k_hasafs() &&
	    (s->authctxt->krb5_ctx != NULL)) {
		char cell[64];

		debug("Getting AFS token");

		k_setpag();

		if (k_afs_cell_of_file(pw->pw_dir, cell, sizeof(cell)) == 0)
			krb5_afslog(s->authctxt->krb5_ctx,
			    s->authctxt->krb5_fwd_ccache, cell, NULL);

		krb5_afslog_home(s->authctxt->krb5_ctx,
		    s->authctxt->krb5_fwd_ccache, NULL, NULL, pw->pw_dir);
	}
#endif

	 
	if (chdir(pw->pw_dir) < 0) {
		 
#ifdef HAVE_LOGIN_CAP
		r = login_getcapbool(lc, "requirehome", 0);
#endif
		if (r || !in_chroot) {
			fprintf(stderr, "Could not chdir to home "
			    "directory %s: %s\n", pw->pw_dir,
			    strerror(errno));
		}
		if (r)
			exit(1);
	}

	closefrom(STDERR_FILENO + 1);

	if (!options.use_login)
		do_rc_files(s, shell);

	 
	signal(SIGPIPE, SIG_DFL);

	if (s->is_subsystem == SUBSYSTEM_INT_SFTP_ERROR) {
		printf("This service allows sftp connections only.\n");
		fflush(NULL);
		exit(1);
	} else if (s->is_subsystem == SUBSYSTEM_INT_SFTP) {
		extern int optind, optreset;
		int i;
		char *p, *args;

		setproctitle("%s@%s", s->pw->pw_name, INTERNAL_SFTP_NAME);
		args = xstrdup(command ? command : "sftp-server");
		for (i = 0, (p = strtok(args, " ")); p; (p = strtok(NULL, " ")))
			if (i < ARGV_MAX - 1)
				argv[i++] = p;
		argv[i] = NULL;
		optind = optreset = 1;
		__progname = argv[0];
#ifdef WITH_SELINUX
		ssh_selinux_change_context("sftpd_t");
#endif
		exit(sftp_server_main(i, argv, s->pw));
	}

	fflush(NULL);

	if (options.use_login) {
		launch_login(pw, hostname);
		 
	}

	 
	if ((shell0 = strrchr(shell, '/')) != NULL)
		shell0++;
	else
		shell0 = shell;

	 
	if (!command) {
		char argv0[256];

		 
		argv0[0] = '-';

		if (strlcpy(argv0 + 1, shell0, sizeof(argv0) - 1)
		    >= sizeof(argv0) - 1) {
			errno = EINVAL;
			perror(shell);
			exit(1);
		}

		 
		argv[0] = argv0;
		argv[1] = NULL;
		execve(shell, argv, env);

		 
		perror(shell);
		exit(1);
	}
	 
	argv[0] = (char *) shell0;
	argv[1] = "-c";
	argv[2] = (char *) command;
	argv[3] = NULL;
	execve(shell, argv, env);
	perror(shell);
	exit(1);
}
