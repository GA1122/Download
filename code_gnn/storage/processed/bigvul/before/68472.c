static void duplicate(char *name) {
	char *fname = check_dir_or_file(name);

	if (arg_debug)
		printf("Private home: duplicating %s\n", fname);
	assert(strncmp(fname, cfg.homedir, strlen(cfg.homedir)) == 0);

	struct stat s;
	if (stat(fname, &s) == -1) {
		free(fname);
		return;
	}
	
	if(nftw(fname, fs_copydir, 1, FTW_PHYS) != 0) {
		fprintf(stderr, "Error: unable to copy template dir\n");
		exit(1);
	}
	fs_logger_print();	 

	free(fname);
}