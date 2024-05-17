int prepare_ramfs_root(char *root)
{
	char buf[LINELEN], *p;
	char nroot[PATH_MAX];
	FILE *f;
	int i;
	char *p2;

	if (realpath(root, nroot) == NULL)
		return -1;

	if (chdir("/") == -1)
		return -1;

	 
	if (mount(root, "/", NULL, MS_REC | MS_BIND, NULL)) {
		SYSERROR("Failed to move %s into /", root);
		return -1;
	}

	if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL)) {
		SYSERROR("Failed to make . rprivate");
		return -1;
	}

	 
	while (1) {
		int progress = 0;

		f = fopen("./proc/self/mountinfo", "r");
		if (!f) {
			SYSERROR("Unable to open /proc/self/mountinfo");
			return -1;
		}
		while (fgets(buf, LINELEN, f)) {
			for (p = buf, i=0; p && i < 4; i++)
				p = strchr(p+1, ' ');
			if (!p)
				continue;
			p2 = strchr(p+1, ' ');
			if (!p2)
				continue;

			*p2 = '\0';
			*p = '.';

			if (strcmp(p + 1, "/") == 0)
				continue;
			if (strcmp(p + 1, "/proc") == 0)
				continue;

			if (umount2(p, MNT_DETACH) == 0)
				progress++;
		}
		fclose(f);
		if (!progress)
			break;
	}

	 
	umount2("./proc", MNT_DETACH);

	 
	if (chdir("..") == -1) {
		SYSERROR("Unable to change working directory");
		return -1;
	}

	if (chroot(".") == -1) {
		SYSERROR("Unable to chroot");
		return -1;
	}

	return 0;
}
