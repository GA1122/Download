static char *check_dir_or_file(const char *name) {
	assert(name);
	struct stat s;

	invalid_filename(name);

	if (arg_debug)
		printf("Private home: checking %s\n", name);

	char *fname = expand_home(name, cfg.homedir);
	if (!fname) {
		fprintf(stderr, "Error: file %s not found.\n", name);
		exit(1);
	}

	if (fname[0] != '/') {
		char* tmp;
		if (asprintf(&tmp, "%s/%s", cfg.homedir, fname) == -1)
			errExit("asprintf");
		free(fname);
		fname = tmp;
	}

	char *rname = realpath(fname, NULL);
	if (!rname) {
		fprintf(stderr, "Error: invalid file %s\n", name);
		exit(1);
	}
	if (strncmp(rname, cfg.homedir, strlen(cfg.homedir)) != 0) {
		fprintf(stderr, "Error: file %s is not in user home directory\n", name);
		exit(1);
	}
	
	if (strcmp(rname, cfg.homedir) == 0) {
		fprintf(stderr, "Error: invalid directory %s\n", rname);
		exit(1);
	}
	
	char *ptr = rname + strlen(cfg.homedir);
	if (*ptr == '\0') {
		fprintf(stderr, "Error: invalid file %s\n", name);
		exit(1);
	}
	ptr++;
	ptr = strchr(ptr, '/');
	if (ptr) {
		if (*ptr != '\0') {
			fprintf(stderr, "Error: only top files and directories in user home are allowed\n");
			exit(1);
		}
	}

	if (stat(fname, &s) == -1) {
		fprintf(stderr, "Error: file %s not found.\n", fname);
		exit(1);
	}

	uid_t uid = getuid();
	gid_t gid = getgid();
	if (s.st_uid != uid || s.st_gid != gid) {
		fprintf(stderr, "Error: only files or directories created by the current user are allowed.\n");
		exit(1);
	}

	if (S_ISDIR(s.st_mode) || S_ISREG(s.st_mode)) {
		free(fname);
		return rname;			   
	}

	fprintf(stderr, "Error: invalid file type, %s.\n", fname);
	exit(1);
}
