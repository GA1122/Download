check_mountpoint(const char *progname, char *mountpoint)
{
	int err;
	struct stat statbuf;

	 
	err = stat(".", &statbuf);
	if (err) {
		fprintf(stderr, "%s: failed to stat %s: %s\n", progname,
				mountpoint, strerror(errno));
		return EX_USAGE;
	}

	if (!S_ISDIR(statbuf.st_mode)) {
		fprintf(stderr, "%s: %s is not a directory!", progname,
				mountpoint);
		return EX_USAGE;
	}

#if CIFS_LEGACY_SETUID_CHECK
	 
	if (!getuid() || geteuid())
		return 0;

	if (statbuf.st_uid != getuid()) {
		fprintf(stderr, "%s: %s is not owned by user\n", progname,
			mountpoint);
		return EX_USAGE;
	}

	if ((statbuf.st_mode & S_IRWXU) != S_IRWXU) {
		fprintf(stderr, "%s: invalid permissions on %s\n", progname,
			mountpoint);
		return EX_USAGE;
	}
#endif  

	return 0;
}
