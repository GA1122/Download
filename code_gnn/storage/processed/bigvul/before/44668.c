int detect_ramfs_rootfs(void)
{
	char buf[LINELEN], *p;
	FILE *f;
	int i;
	char *p2;

	f = fopen("/proc/self/mountinfo", "r");
	if (!f)
		return 0;
	while (fgets(buf, LINELEN, f)) {
		for (p = buf, i=0; p && i < 4; i++)
			p = strchr(p+1, ' ');
		if (!p)
			continue;
		p2 = strchr(p+1, ' ');
		if (!p2)
			continue;
		*p2 = '\0';
		if (strcmp(p+1, "/") == 0) {
			p = strchr(p2+1, '-');
			if (p && strncmp(p, "- rootfs rootfs ", 16) == 0) {
				fclose(f);
				return 1;
			}
		}
	}
	fclose(f);
	return 0;
}