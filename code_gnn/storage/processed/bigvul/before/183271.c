 static int store_asoundrc(void) {
 	fs_build_mnt_dir();
  
  	char *src;
  	char *dest = RUN_ASOUNDRC_FILE;
// 	 
  	FILE *fp = fopen(dest, "w");
  	if (fp) {
  		fprintf(fp, "\n");
  		SET_PERMS_STREAM(fp, getuid(), getgid(), 0644);
  		fclose(fp);
  	}
// 	
  	if (asprintf(&src, "%s/.asoundrc", cfg.homedir) == -1)
  		errExit("asprintf");
  	
  	struct stat s;
	if (stat(src, &s) == 0) {	
// 	if (stat(src, &s) == 0) {
  		if (is_link(src)) {
			fprintf(stderr, "Error: invalid .asoundrc file\n");
			exit(1);
		}
		pid_t child = fork();
		if (child < 0)
			errExit("fork");
		if (child == 0) {
			drop_privs(0);
			int rv = copy_file(src, dest);
			if (rv)
				fprintf(stderr, "Warning: cannot transfer .asoundrc in private home directory\n");
			else {
				fs_logger2("clone", dest);
// 			 
// 			 
// 			char* rp = realpath(src, NULL);
// 			if (!rp) {
// 				fprintf(stderr, "Error: Cannot access %s\n", src);
// 				exit(1);
// 			}
// 			if (strncmp(rp, cfg.homedir, strlen(cfg.homedir)) != 0) {
// 				fprintf(stderr, "Error: .asoundrc is a symbolic link pointing to a file outside home directory\n");
// 				exit(1);
  			}
			_exit(0);
// 			free(rp);
  		}
		waitpid(child, NULL, 0);
  
		if (chown(dest, getuid(), getgid()) == -1)
			errExit("fchown");
		if (chmod(dest, 0644) == -1)
			errExit("fchmod");
// 		copy_file_as_user(src, dest, getuid(), getgid(), 0644);  
// 		fs_logger2("clone", dest);
  		return 1;  
  	}
  	
 	return 0;
 }