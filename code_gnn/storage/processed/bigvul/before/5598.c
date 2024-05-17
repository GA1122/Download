pam_sm_open_session (pam_handle_t *pamh, int flags UNUSED,
		     int argc, const char **argv)
{
	char *cookiefile = NULL, *xauthority = NULL,
	     *cookie = NULL, *display = NULL, *tmp = NULL,
	     *xauthlocalhostname = NULL;
	const char *user, *xauth = NULL;
	struct passwd *tpwd, *rpwd;
	int fd, i, debug = 0;
	int retval = PAM_SUCCESS;
	uid_t systemuser = 499, targetuser = 0, fsuid;

	 
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "debug") == 0) {
			debug = 1;
			continue;
		}
		if (strncmp(argv[i], "xauthpath=", 10) == 0) {
			xauth = argv[i] + 10;
			continue;
		}
		if (strncmp(argv[i], "targetuser=", 11) == 0) {
			long l = strtol(argv[i] + 11, &tmp, 10);
			if ((strlen(argv[i] + 11) > 0) && (*tmp == '\0')) {
				targetuser = l;
			} else {
				pam_syslog(pamh, LOG_WARNING,
					   "invalid value for targetuser (`%s')",
					   argv[i] + 11);
			}
			continue;
		}
		if (strncmp(argv[i], "systemuser=", 11) == 0) {
			long l = strtol(argv[i] + 11, &tmp, 10);
			if ((strlen(argv[i] + 11) > 0) && (*tmp == '\0')) {
				systemuser = l;
			} else {
				pam_syslog(pamh, LOG_WARNING,
					   "invalid value for systemuser (`%s')",
					   argv[i] + 11);
			}
			continue;
		}
		pam_syslog(pamh, LOG_WARNING, "unrecognized option `%s'",
			   argv[i]);
	}

	if (xauth == NULL) {
	        size_t j;
		for (j = 0; j < sizeof(xauthpaths)/sizeof(xauthpaths[0]); j++) {
			if (access(xauthpaths[j], X_OK) == 0) {
				xauth = xauthpaths[j];
				break;
			}
		}
		if (xauth == NULL) {
			 
			return PAM_SUCCESS;
		}
	}

	 
	if ((display = getenv("DISPLAY")) == NULL) {
		if (debug) {
			pam_syslog(pamh, LOG_DEBUG,
				   "user has no DISPLAY, doing nothing");
		}
		return PAM_SUCCESS;
	}

	 
	if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS) {
		pam_syslog(pamh, LOG_ERR,
			   "error determining target user's name");
		retval = PAM_SESSION_ERR;
		goto cleanup;
	}
	rpwd = pam_modutil_getpwuid(pamh, getuid());
	if (rpwd == NULL) {
		pam_syslog(pamh, LOG_ERR,
			   "error determining invoking user's name");
		retval = PAM_SESSION_ERR;
		goto cleanup;
	}

	 
	tpwd = pam_modutil_getpwnam(pamh, user);
	if (tpwd == NULL) {
		pam_syslog(pamh, LOG_ERR,
			   "error determining target user's UID");
		retval = PAM_SESSION_ERR;
		goto cleanup;
	}

	if (debug) {
		pam_syslog(pamh, LOG_DEBUG,
			   "requesting user %lu/%lu, target user %lu/%lu",
			   (unsigned long) rpwd->pw_uid,
			   (unsigned long) rpwd->pw_gid,
			   (unsigned long) tpwd->pw_uid,
			   (unsigned long) tpwd->pw_gid);
	}

	 
	if ((tpwd->pw_uid != 0) &&
	    (tpwd->pw_uid != targetuser) &&
	    (tpwd->pw_uid <= systemuser)) {
		if (debug) {
			pam_syslog(pamh, LOG_DEBUG,
				   "not forwarding cookies to user ID %lu",
				   (unsigned long) tpwd->pw_uid);
		}
		retval = PAM_SESSION_ERR;
		goto cleanup;
	}

	 
	i = (getuid() != 0 || tpwd->pw_uid == 0) ? PAM_SUCCESS : PAM_PERM_DENIED;
	i = check_acl(pamh, "export", rpwd->pw_name, user, i, debug);
	if (i != PAM_SUCCESS) {
		retval = PAM_SESSION_ERR;
		goto cleanup;
	}
	i = PAM_SUCCESS;
	i = check_acl(pamh, "import", user, rpwd->pw_name, i, debug);
	if (i != PAM_SUCCESS) {
		retval = PAM_SESSION_ERR;
		goto cleanup;
	}

	 
	if (getenv(XAUTHENV) != NULL) {
		cookiefile = strdup(getenv(XAUTHENV));
	} else {
		cookiefile = malloc(strlen(rpwd->pw_dir) + 1 +
				    strlen(XAUTHDEF) + 1);
		if (cookiefile == NULL) {
			retval = PAM_SESSION_ERR;
			goto cleanup;
		}
		strcpy(cookiefile, rpwd->pw_dir);
		strcat(cookiefile, "/");
		strcat(cookiefile, XAUTHDEF);
	}
	if (debug) {
		pam_syslog(pamh, LOG_DEBUG, "reading keys from `%s'",
			   cookiefile);
	}

	 
	if (debug) {
		pam_syslog(pamh, LOG_DEBUG,
			   "running \"%s %s %s %s %s\" as %lu/%lu",
			   xauth, "-f", cookiefile, "nlist", display,
			   (unsigned long) getuid(), (unsigned long) getgid());
	}
	if (run_coprocess(pamh, NULL, &cookie,
			  getuid(), getgid(),
			  xauth, "-f", cookiefile, "nlist", display,
			  NULL) == 0) {
		int save_errno;
#ifdef WITH_SELINUX
		security_context_t context = NULL;
#endif
		 
		if (((cookie == NULL) || (strlen(cookie) == 0)) &&
		    ((strncmp(display, "localhost:", 10) == 0) ||
		     (strncmp(display, "localhost/unix:", 15) == 0))) {
			char *t, *screen;
			size_t tlen, slen;
			 
			if (cookie != NULL) {
				free(cookie);
				cookie = NULL;
			}
			 
			tlen = strlen(display) + LINE_MAX + 1;
			t = malloc(tlen);
			if (t != NULL) {
				memset(t, 0, tlen);
				if (gethostname(t, tlen - 1) != -1) {
					 
					if (strlen(t) < tlen - 6) {
						strcat(t, "/unix:");
					}
					screen = strchr(display, ':');
					if (screen != NULL) {
						screen++;
						slen = strlen(screen);
						if (strlen(t) + slen < tlen) {
							strcat(t, screen);
						}
					}
					if (debug) {
						pam_syslog(pamh, LOG_DEBUG,
							   "no key for `%s', "
							   "trying `%s'",
							   display, t);
					}
					 
					if (debug) {
						pam_syslog(pamh, LOG_DEBUG,
						       "running "
						       "\"%s %s %s %s %s\" as "
						       "%lu/%lu",
						       xauth,
						       "-f",
						       cookiefile,
						       "nlist",
						       t,
						       (unsigned long) getuid(),
						       (unsigned long) getgid());
					}
					run_coprocess(pamh, NULL, &cookie,
						      getuid(), getgid(),
						      xauth, "-f", cookiefile,
						      "nlist", t, NULL);
				}
				free(t);
				t = NULL;
			}
		}

		 
		if ((cookie == NULL) || (strlen(cookie) == 0)) {
			if (debug) {
				pam_syslog(pamh, LOG_DEBUG, "no key");
			}
			retval = PAM_SESSION_ERR;
			goto cleanup;
		}

		 
		if (asprintf(&xauthority, "%s=%s/%s",
			     XAUTHENV, tpwd->pw_dir, XAUTHTMP) < 0) {
			xauthority = NULL;
			if (debug) {
				pam_syslog(pamh, LOG_DEBUG, "out of memory");
			}
			retval = PAM_SESSION_ERR;
			goto cleanup;
		}

		 
		fsuid = setfsuid(tpwd->pw_uid);

#ifdef WITH_SELINUX
		if (is_selinux_enabled() > 0) {
			struct selabel_handle *ctx = selabel_open(SELABEL_CTX_FILE, NULL, 0);
			if (ctx != NULL) {
				if (selabel_lookup(ctx, &context,
						   xauthority + sizeof(XAUTHENV), S_IFREG) != 0) {
					pam_syslog(pamh, LOG_WARNING,
						   "could not get SELinux label for '%s'",
						   xauthority + sizeof(XAUTHENV));
				}
				selabel_close(ctx);
				if (setfscreatecon(context)) {
					pam_syslog(pamh, LOG_WARNING,
						   "setfscreatecon(%s) failed: %m", context);
				}
			}
		}
		fd = mkstemp(xauthority + sizeof(XAUTHENV));
		save_errno = errno;
		if (context != NULL) {
			free(context);
			setfscreatecon(NULL);
		}
#else
		fd = mkstemp(xauthority + sizeof(XAUTHENV));
		save_errno = errno;
#endif

		setfsuid(fsuid);
		if (fd == -1) {
			errno = save_errno;
			pam_syslog(pamh, LOG_ERR,
				   "error creating temporary file `%s': %m",
				   xauthority + sizeof(XAUTHENV));
			retval = PAM_SESSION_ERR;
			goto cleanup;
		}
		 
		setfsuid(tpwd->pw_uid);
		if (fchown(fd, tpwd->pw_uid, tpwd->pw_gid) < 0)
		  pam_syslog (pamh, LOG_ERR, "fchown: %m");
		setfsuid(fsuid);
		close(fd);

		 
		free(cookiefile);
		cookiefile = strdup(xauthority + sizeof(XAUTHENV));

		 
		if (pam_set_data(pamh, DATANAME, cookiefile, cleanup) != PAM_SUCCESS) {
			pam_syslog(pamh, LOG_ERR,
				   "error saving name of temporary file `%s'",
				   cookiefile);
			unlink(cookiefile);
			retval = PAM_SESSION_ERR;
			goto cleanup;
		}

		 
		if (pam_putenv (pamh, xauthority) != PAM_SUCCESS)
			pam_syslog(pamh, LOG_ERR,
				   "can't set environment variable '%s'",
				   xauthority);
		putenv (xauthority);  
		xauthority = NULL;  

		 
		{
		  char *d;

		  if (asprintf(&d, "DISPLAY=%s", display) < 0)
		    {
		      pam_syslog(pamh, LOG_ERR, "out of memory");
		      cookiefile = NULL;
		      retval = PAM_SESSION_ERR;
		      goto cleanup;
		    }

		  if (pam_putenv (pamh, d) != PAM_SUCCESS)
		    pam_syslog (pamh, LOG_ERR,
				"can't set environment variable '%s'", d);
		  free (d);
		}

		 
		if ((xauthlocalhostname = getenv("XAUTHLOCALHOSTNAME")) != NULL) {
		  char *d;

		  if (asprintf(&d, "XAUTHLOCALHOSTNAME=%s", xauthlocalhostname) < 0) {
		    pam_syslog(pamh, LOG_ERR, "out of memory");
		    retval = PAM_SESSION_ERR;
		    goto cleanup;
		  }

		  if (pam_putenv (pamh, d) != PAM_SUCCESS)
		    pam_syslog (pamh, LOG_ERR,
				"can't set environment variable '%s'", d);
		  free (d);
		}

		 
		if (debug) {
			pam_syslog(pamh, LOG_DEBUG,
				   "writing key `%s' to temporary file `%s'",
				   cookie, cookiefile);
		}
		if (debug) {
			pam_syslog(pamh, LOG_DEBUG,
				  "running \"%s %s %s %s %s\" as %lu/%lu",
				  xauth, "-f", cookiefile, "nmerge", "-",
				  (unsigned long) tpwd->pw_uid,
				  (unsigned long) tpwd->pw_gid);
		}
		run_coprocess(pamh, cookie, &tmp,
			      tpwd->pw_uid, tpwd->pw_gid,
			      xauth, "-f", cookiefile, "nmerge", "-", NULL);

		 
		cookiefile = NULL;
		free(tmp);
	}
cleanup:
	 
	if (retval != PAM_SUCCESS && getenv (XAUTHENV))
		unsetenv (XAUTHENV);
	free(cookiefile);
	free(cookie);
	free(xauthority);
	return retval;
}
