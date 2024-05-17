static void chk_chroot(void) {
	char *mycont = getenv("container");
	if (mycont)
		return;

	struct stat s;
	if (stat("/", &s) == 0) {
		if (s.st_ino != 2)
			return;
	}

	fprintf(stderr, "Error: cannot mount filesystem as slave\n");
	exit(1);
}
