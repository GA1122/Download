privsep_preauth_child(void)
{
	gid_t gidset[1];
	struct passwd *pw;

	 
	privsep_challenge_enable();

#ifdef GSSAPI
	 
	if (options.gss_authentication)
		ssh_gssapi_prepare_supported_oids();
#endif

	 
	demote_sensitive_data();

	 
	if (getuid() == 0 || geteuid() == 0) {
		if ((pw = getpwnam(SSH_PRIVSEP_USER)) == NULL)
			fatal("Privilege separation user %s does not exist",
			    SSH_PRIVSEP_USER);
		explicit_bzero(pw->pw_passwd, strlen(pw->pw_passwd));
		endpwent();

		 
		if (chroot(_PATH_PRIVSEP_CHROOT_DIR) == -1)
			fatal("chroot(\"%s\"): %s", _PATH_PRIVSEP_CHROOT_DIR,
			    strerror(errno));
		if (chdir("/") == -1)
			fatal("chdir(\"/\"): %s", strerror(errno));

		 
		debug3("privsep user:group %u:%u", (u_int)pw->pw_uid,
		    (u_int)pw->pw_gid);
		gidset[0] = pw->pw_gid;
		if (setgroups(1, gidset) < 0)
			fatal("setgroups: %.100s", strerror(errno));
		permanently_set_uid(pw);
	}
}
