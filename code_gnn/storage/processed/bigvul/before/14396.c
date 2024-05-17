do_setusercontext(struct passwd *pw)
{
	char *chroot_path, *tmp;

	platform_setusercontext(pw);

	if (platform_privileged_uidswap()) {
#ifdef HAVE_LOGIN_CAP
		if (setusercontext(lc, pw, pw->pw_uid,
		    (LOGIN_SETALL & ~(LOGIN_SETPATH|LOGIN_SETUSER))) < 0) {
			perror("unable to set user context");
			exit(1);
		}
#else
		if (setlogin(pw->pw_name) < 0)
			error("setlogin failed: %s", strerror(errno));
		if (setgid(pw->pw_gid) < 0) {
			perror("setgid");
			exit(1);
		}
		 
		if (initgroups(pw->pw_name, pw->pw_gid) < 0) {
			perror("initgroups");
			exit(1);
		}
		endgrent();
#endif

		platform_setusercontext_post_groups(pw);

		if (!in_chroot && options.chroot_directory != NULL &&
		    strcasecmp(options.chroot_directory, "none") != 0) {
                        tmp = tilde_expand_filename(options.chroot_directory,
			    pw->pw_uid);
			chroot_path = percent_expand(tmp, "h", pw->pw_dir,
			    "u", pw->pw_name, (char *)NULL);
			safely_chroot(chroot_path, pw->pw_uid);
			free(tmp);
			free(chroot_path);
			 
			free(options.chroot_directory);
			options.chroot_directory = NULL;
			in_chroot = 1;
		}

#ifdef HAVE_LOGIN_CAP
		if (setusercontext(lc, pw, pw->pw_uid, LOGIN_SETUSER) < 0) {
			perror("unable to set user context (setuser)");
			exit(1);
		}
		 
		(void) setusercontext(lc, pw, pw->pw_uid, LOGIN_SETUMASK);
#else
# ifdef USE_LIBIAF
		 
		if (!in_chroot && set_id(pw->pw_name) != 0)
			fatal("set_id(%s) Failed", pw->pw_name);
# endif  
		 
		permanently_set_uid(pw);
#endif
	} else if (options.chroot_directory != NULL &&
	    strcasecmp(options.chroot_directory, "none") != 0) {
		fatal("server lacks privileges to chroot to ChrootDirectory");
	}

	if (getuid() != pw->pw_uid || geteuid() != pw->pw_uid)
		fatal("Failed to set uids to %u.", (u_int) pw->pw_uid);
}
