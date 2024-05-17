void fs_private(void) {
	char *homedir = cfg.homedir;
	assert(homedir);
	uid_t u = getuid();
	gid_t g = getgid();

	int xflag = store_xauthority();
	int aflag = store_asoundrc();

	if (arg_debug)
		printf("Mounting a new /home directory\n");
	if (mount("tmpfs", "/home", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_STRICTATIME | MS_REC,  "mode=755,gid=0") < 0)
		errExit("mounting home directory");
	fs_logger("tmpfs /home");

	if (arg_debug)
		printf("Mounting a new /root directory\n");
	if (mount("tmpfs", "/root", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_STRICTATIME | MS_REC,  "mode=700,gid=0") < 0)
		errExit("mounting root directory");
	fs_logger("tmpfs /root");

	if (u != 0) {
		if (arg_debug)
			printf("Create a new user directory\n");
		if (mkdir(homedir, S_IRWXU) == -1) {
			if (mkpath_as_root(homedir) == -1)
				errExit("mkpath");
			if (mkdir(homedir, S_IRWXU) == -1)
				errExit("mkdir");
		}
		if (chown(homedir, u, g) < 0)
			errExit("chown");
		fs_logger2("mkdir", homedir);
	}
	
	skel(homedir, u, g);
	if (xflag)
		copy_xauthority();
	if (aflag)
		copy_asoundrc();

}