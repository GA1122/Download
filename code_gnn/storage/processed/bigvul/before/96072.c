void drop_privs(int nogroups) {
	EUID_ROOT();
	gid_t gid = getgid();

	if (gid == 0 || nogroups) {
		if (setgroups(0, NULL) < 0)
			errExit("setgroups");
		if (arg_debug)
			printf("Username %s, no supplementary groups\n", cfg.username);
	}
	else {
		assert(cfg.username);
		gid_t groups[MAX_GROUPS];
		int ngroups = MAX_GROUPS;
		int rv = getgrouplist(cfg.username, gid, groups, &ngroups);

		if (arg_debug && rv) {
			printf("Username %s, groups ", cfg.username);
			int i;
			for (i = 0; i < ngroups; i++)
				printf("%u, ", groups[i]);
			printf("\n");
		}

		if (rv == -1) {
			fprintf(stderr, "Warning: cannot extract supplementary group list, dropping them\n");
			if (setgroups(0, NULL) < 0)
				errExit("setgroups");
		}
		else {
			rv = setgroups(ngroups, groups);
			if (rv) {
				fprintf(stderr, "Warning: cannot set supplementary group list, dropping them\n");
				if (setgroups(0, NULL) < 0)
					errExit("setgroups");
			}
		}
	}

	if (setgid(getgid()) < 0)
		errExit("setgid/getgid");
	if (setuid(getuid()) < 0)
		errExit("setuid/getuid");
}