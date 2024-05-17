uucp_lock(void)
{
	int r, fd, pid;
	char buf[16];
	mode_t m;

	if ( lockname[0] == '\0' ) return 0;

	fd = open(lockname, O_RDONLY);
	if ( fd >= 0 ) {
		r = read(fd, buf, sizeof(buf)); 
		close(fd);
		 
		pid = (r == 4) ? *(int *)buf : strtol(buf, NULL, 10);
		if ( pid > 0 
			 && kill((pid_t)pid, 0) < 0 
			 && errno == ESRCH ) {
			 
			printf("Removing stale lock: %s\n", lockname);
			sleep(1);
			unlink(lockname);
		} else {
			lockname[0] = '\0';
			errno = EEXIST;
			return -1;
		}
	}
	 
	m = umask(022);
	fd = open(lockname, O_WRONLY|O_CREAT|O_EXCL, 0666);
	if ( fd < 0 ) { lockname[0] = '\0'; return -1; }
	umask(m);
	snprintf(buf, sizeof(buf), "%04d\n", getpid());
	write(fd, buf, strlen(buf));
	close(fd);

	return 0;
}