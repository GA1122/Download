do_nologin(struct passwd *pw)
{
	FILE *f = NULL;
	char buf[1024], *nl, *def_nl = _PATH_NOLOGIN;
	struct stat sb;

#ifdef HAVE_LOGIN_CAP
	if (login_getcapbool(lc, "ignorenologin", 0) || pw->pw_uid == 0)
		return;
	nl = login_getcapstr(lc, "nologin", def_nl, def_nl);
#else
	if (pw->pw_uid == 0)
		return;
	nl = def_nl;
#endif
	if (stat(nl, &sb) == -1) {
		if (nl != def_nl)
			free(nl);
		return;
	}

	 
	logit("User %.100s not allowed because %s exists", pw->pw_name, nl);
	if ((f = fopen(nl, "r")) != NULL) {
 		while (fgets(buf, sizeof(buf), f))
 			fputs(buf, stderr);
 		fclose(f);
 	}
	exit(254);
}
