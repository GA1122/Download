 do_setup_env(Session *s, const char *shell)
 {
 	struct ssh *ssh = active_state;  
 	char buf[256];
 	u_int i, envsize;
 	char **env, *laddr;
 	struct passwd *pw = s->pw;
 #if !defined (HAVE_LOGIN_CAP) && !defined (HAVE_CYGWIN)
 	char *path = NULL;
 #endif
 
 	 
 	envsize = 100;
 	env = xcalloc(envsize, sizeof(char *));
 	env[0] = NULL;
 
 #ifdef HAVE_CYGWIN
 	 
 	{
 		char **p;
 
 		p = fetch_windows_environment();
 		copy_environment(p, &env, &envsize);
 		free_windows_environment(p);
 	}
 #endif
 
 #ifdef GSSAPI
 	 
 	ssh_gssapi_do_child(&env, &envsize);
 #endif
 
 	if (!options.use_login) {
 		 
 		for (i = 0; i < s->num_env; i++)
 			child_set_env(&env, &envsize, s->env[i].name,
 			    s->env[i].val);
 
 		child_set_env(&env, &envsize, "USER", pw->pw_name);
 		child_set_env(&env, &envsize, "LOGNAME", pw->pw_name);
 #ifdef _AIX
 		child_set_env(&env, &envsize, "LOGIN", pw->pw_name);
 #endif
 		child_set_env(&env, &envsize, "HOME", pw->pw_dir);
 #ifdef HAVE_LOGIN_CAP
 		if (setusercontext(lc, pw, pw->pw_uid, LOGIN_SETPATH) < 0)
 			child_set_env(&env, &envsize, "PATH", _PATH_STDPATH);
 		else
 			child_set_env(&env, &envsize, "PATH", getenv("PATH"));
 #else  
 # ifndef HAVE_CYGWIN
 		 
 #  ifdef HAVE_ETC_DEFAULT_LOGIN
 		read_etc_default_login(&env, &envsize, pw->pw_uid);
 		path = child_get_env(env, "PATH");
 #  endif  
 		if (path == NULL || *path == '\0') {
 			child_set_env(&env, &envsize, "PATH",
 			    s->pw->pw_uid == 0 ?
 				SUPERUSER_PATH : _PATH_STDPATH);
 		}
 # endif  
 #endif  
 
 		snprintf(buf, sizeof buf, "%.200s/%.50s",
 			 _PATH_MAILDIR, pw->pw_name);
 		child_set_env(&env, &envsize, "MAIL", buf);
 
 		 
 		child_set_env(&env, &envsize, "SHELL", shell);
 	}
 	if (getenv("TZ"))
 		child_set_env(&env, &envsize, "TZ", getenv("TZ"));
 
 	 
 	if (!options.use_login) {
 		while (custom_environment) {
 			struct envstring *ce = custom_environment;
 			char *str = ce->s;
 
 			for (i = 0; str[i] != '=' && str[i]; i++)
 				;
 			if (str[i] == '=') {
 				str[i] = 0;
 				child_set_env(&env, &envsize, str, str + i + 1);
 			}
 			custom_environment = ce->next;
 			free(ce->s);
 			free(ce);
 		}
 	}
 
 	 
 	snprintf(buf, sizeof buf, "%.50s %d %d",
 	    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh),
 	    ssh_local_port(ssh));
 	child_set_env(&env, &envsize, "SSH_CLIENT", buf);
 
 	laddr = get_local_ipaddr(packet_get_connection_in());
 	snprintf(buf, sizeof buf, "%.50s %d %.50s %d",
 	    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh),
 	    laddr, ssh_local_port(ssh));
 	free(laddr);
 	child_set_env(&env, &envsize, "SSH_CONNECTION", buf);
 
 	if (s->ttyfd != -1)
 		child_set_env(&env, &envsize, "SSH_TTY", s->tty);
 	if (s->term)
 		child_set_env(&env, &envsize, "TERM", s->term);
 	if (s->display)
 		child_set_env(&env, &envsize, "DISPLAY", s->display);
 	if (original_command)
 		child_set_env(&env, &envsize, "SSH_ORIGINAL_COMMAND",
 		    original_command);
 
 #ifdef _UNICOS
 	if (cray_tmpdir[0] != '\0')
 		child_set_env(&env, &envsize, "TMPDIR", cray_tmpdir);
 #endif  
 
 	 
 	{
 		char *cp;
 
 		if ((cp = getenv("KRB5CCNAME")) != NULL)
 			child_set_env(&env, &envsize, "KRB5CCNAME", cp);
 	}
 
 #ifdef _AIX
 	{
 		char *cp;
 
 		if ((cp = getenv("AUTHSTATE")) != NULL)
 			child_set_env(&env, &envsize, "AUTHSTATE", cp);
 		read_environment_file(&env, &envsize, "/etc/environment");
 	}
 #endif
 #ifdef KRB5
 	if (s->authctxt->krb5_ccname)
 		child_set_env(&env, &envsize, "KRB5CCNAME",
 		    s->authctxt->krb5_ccname);
 #endif
 #ifdef USE_PAM
 	 
	if (options.use_pam) {
// 	if (options.use_pam && !options.use_login) {
  		char **p;
  
  		p = fetch_pam_child_environment();
 		copy_environment(p, &env, &envsize);
 		free_pam_environment(p);
 
 		p = fetch_pam_environment();
 		copy_environment(p, &env, &envsize);
 		free_pam_environment(p);
 	}
 #endif  
 
 	if (auth_sock_name != NULL)
 		child_set_env(&env, &envsize, SSH_AUTHSOCKET_ENV_NAME,
 		    auth_sock_name);
 
 	 
 	if (options.permit_user_env && !options.use_login) {
 		snprintf(buf, sizeof buf, "%.200s/.ssh/environment",
 		    strcmp(pw->pw_dir, "/") ? pw->pw_dir : "");
 		read_environment_file(&env, &envsize, buf);
 	}
 	if (debug_flag) {
 		 
 		fprintf(stderr, "Environment:\n");
 		for (i = 0; env[i]; i++)
 			fprintf(stderr, "  %.200s\n", env[i]);
 	}
 	return env;
 }