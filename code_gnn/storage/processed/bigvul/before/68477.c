void fs_private_home_list(void) {
	char *homedir = cfg.homedir;
	char *private_list = cfg.home_private_keep;
	assert(homedir);
	assert(private_list);

	int xflag = store_xauthority();
	int aflag = store_asoundrc();

	uid_t u = firejail_uid;
	gid_t g = firejail_gid;
	struct stat s;
	if (stat(homedir, &s) == -1) {
		fprintf(stderr, "Error: cannot find user home directory\n");
		exit(1);
	}

	fs_build_mnt_dir();
	int rv = mkdir(RUN_HOME_DIR, 0755);
	if (rv == -1)
		errExit("mkdir");
	if (chown(RUN_HOME_DIR, u, g) < 0)
		errExit("chown");
	if (chmod(RUN_HOME_DIR, 0755) < 0)
		errExit("chmod");
	ASSERT_PERMS(RUN_HOME_DIR, u, g, 0755);

	fs_logger_print();	 

	pid_t child = fork();
	if (child < 0)
		errExit("fork");
	if (child == 0) {
		if (arg_debug)
			printf("Copying files in the new home:\n");

		if (setgroups(0, NULL) < 0)
			errExit("setgroups");
		if (setgid(getgid()) < 0)
			errExit("setgid/getgid");
		if (setuid(getuid()) < 0)
			errExit("setuid/getuid");

		char *dlist = strdup(cfg.home_private_keep);
		if (!dlist)
			errExit("strdup");
		
		char *ptr = strtok(dlist, ",");
		duplicate(ptr);
		while ((ptr = strtok(NULL, ",")) != NULL)
			duplicate(ptr);

		if (!arg_quiet) {
			if (size_limit_reached)
				fprintf(stderr, "Warning: private-home copy limit of %u MB reached, not all the files were copied\n", 
					PRIVATE_COPY_LIMIT / (1024 *1024));
			else
				printf("Private home: %u files, total size %u bytes\n", file_cnt, size_cnt);
		}

		fs_logger_print();	 
		free(dlist);
		_exit(0);
	}
	waitpid(child, NULL, 0);

	if (arg_debug)
		printf("Mount-bind %s on top of %s\n", RUN_HOME_DIR, homedir);

	if (mount(RUN_HOME_DIR, homedir, NULL, MS_BIND|MS_REC, NULL) < 0)
		errExit("mount bind");

	if (u != 0) {
		if (arg_debug)
			printf("Mounting a new /root directory\n");
		if (mount("tmpfs", "/root", "tmpfs", MS_NOSUID | MS_NODEV | MS_STRICTATIME | MS_REC,  "mode=700,gid=0") < 0)
			errExit("mounting home directory");
	}
	else {
		if (arg_debug)
			printf("Mounting a new /home directory\n");
		if (mount("tmpfs", "/home", "tmpfs", MS_NOSUID | MS_NODEV | MS_STRICTATIME | MS_REC,  "mode=755,gid=0") < 0)
			errExit("mounting home directory");
	}

	skel(homedir, u, g);
	if (xflag)
		copy_xauthority();
	if (aflag)
		copy_asoundrc();
}
