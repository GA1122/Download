int copy_file(const char *srcname, const char *destname, uid_t uid, gid_t gid, mode_t mode) {
	assert(srcname);
	assert(destname);

	int src = open(srcname, O_RDONLY);
	if (src < 0) {
		fprintf(stderr, "Warning: cannot open %s, file not copied\n", srcname);
		return -1;
	}

	int dst = open(destname, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (dst < 0) {
		fprintf(stderr, "Warning: cannot open %s, file not copied\n", destname);
		close(src);
		return -1;
	}

	ssize_t len;
	static const int BUFLEN = 1024;
	unsigned char buf[BUFLEN];
	while ((len = read(src, buf, BUFLEN)) > 0) {
		int done = 0;
		while (done != len) {
			int rv = write(dst, buf + done, len - done);
			if (rv == -1) {
				close(src);
				close(dst);
				return -1;
			}

			done += rv;
		}
	}

	if (fchown(dst, uid, gid) == -1)
		errExit("fchown");
	if (fchmod(dst, mode) == -1)
		errExit("fchmod");

	close(src);
	close(dst);
 	return 0;
 }