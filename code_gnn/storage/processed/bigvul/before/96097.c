void fs_check_private_dir(void) {
	invalid_filename(cfg.home_private);
	
	char *tmp = expand_home(cfg.home_private, cfg.homedir);
	cfg.home_private = realpath(tmp, NULL);
	free(tmp);
	
	if (!cfg.home_private
	 || !is_dir(cfg.home_private)
	 || is_link(cfg.home_private)
	 || strstr(cfg.home_private, "..")) {
		fprintf(stderr, "Error: invalid private directory\n");
		exit(1);
	}

	struct stat s2;
	int rv = stat(cfg.home_private, &s2);
	if (rv < 0) {
		fprintf(stderr, "Error: cannot find %s directory\n", cfg.home_private);
		exit(1);
	}

	struct stat s1;
	rv = stat(cfg.homedir, &s1);
	if (rv < 0) {
		fprintf(stderr, "Error: cannot find %s directory, full path name required\n", cfg.homedir);
		exit(1);
	}
	if (s1.st_uid != s2.st_uid) {
		printf("Error: --private directory should be owned by the current user\n");
		exit(1);
	}
}