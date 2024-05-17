update_core_dump_pattern(const char *pattern_str)
{
	int fd;
	bool initialising = (orig_core_dump_pattern == NULL);

	 
#define	CORENAME_MAX_SIZE	128

	if (initialising)
		orig_core_dump_pattern = MALLOC(CORENAME_MAX_SIZE);

	fd = open ("/proc/sys/kernel/core_pattern", O_RDWR);

	if (fd == -1 ||
	    (initialising && read(fd, orig_core_dump_pattern, CORENAME_MAX_SIZE - 1) == -1) ||
	    write(fd, pattern_str, strlen(pattern_str)) == -1) {
		log_message(LOG_INFO, "Unable to read/write core_pattern");

		if (fd != -1)
			close(fd);

		FREE(orig_core_dump_pattern);

		return;
	}

	close(fd);

	if (!initialising)
		FREE_PTR(orig_core_dump_pattern);
}
