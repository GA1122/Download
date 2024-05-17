_pam_parse (const pam_handle_t *pamh, int flags, int argc,
	    const char **argv, const char **maildir, size_t *hashcount)
{
    int ctrl=0;

    if (flags & PAM_SILENT) {
	ctrl |= PAM_MAIL_SILENT;
    }

    *hashcount = 0;

     
    for (; argc-- > 0; ++argv) {

	 

	if (!strcmp(*argv,"debug"))
	    ctrl |= PAM_DEBUG_ARG;
	else if (!strcmp(*argv,"quiet"))
	    ctrl |= PAM_QUIET_MAIL;
	else if (!strcmp(*argv,"standard"))
	    ctrl |= PAM_STANDARD_MAIL | PAM_EMPTY_TOO;
	else if (!strncmp(*argv,"dir=",4)) {
	    *maildir = 4 + *argv;
	    if (**maildir != '\0') {
		D(("new mail directory: %s", *maildir));
		ctrl |= PAM_NEW_MAIL_DIR;
	    } else {
		pam_syslog(pamh, LOG_ERR,
			   "dir= specification missing argument - ignored");
	    }
	} else if (!strncmp(*argv,"hash=",5)) {
	    char *ep = NULL;
	    *hashcount = strtoul(*argv+5,&ep,10);
	    if (!ep) {
		*hashcount = 0;
	    }
	} else if (!strcmp(*argv,"close")) {
	    ctrl |= PAM_LOGOUT_TOO;
	} else if (!strcmp(*argv,"nopen")) {
	    ctrl |= PAM_NO_LOGIN;
	} else if (!strcmp(*argv,"noenv")) {
	    ctrl |= PAM_NO_ENV;
	} else if (!strcmp(*argv,"empty")) {
	    ctrl |= PAM_EMPTY_TOO;
	} else {
	    pam_syslog(pamh, LOG_ERR, "unknown option: %s", *argv);
	}
    }

    if ((*hashcount != 0) && !(ctrl & PAM_NEW_MAIL_DIR)) {
	*maildir = DEFAULT_MAIL_DIRECTORY;
	ctrl |= PAM_NEW_MAIL_DIR;
    }

    return ctrl;
}
