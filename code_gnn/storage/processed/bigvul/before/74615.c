void blkid_read_cache(blkid_cache cache)
{
	FILE *file;
	char buf[4096];
	int fd, lineno = 0;
	struct stat st;

	if (!cache)
		return;

	 
	if ((fd = open(cache->bic_filename, O_RDONLY|O_CLOEXEC)) < 0)
		return;
	if (fstat(fd, &st) < 0)
		goto errout;
	if ((st.st_mtime == cache->bic_ftime) ||
	    (cache->bic_flags & BLKID_BIC_FL_CHANGED)) {
		DBG(CACHE, ul_debug("skipping re-read of %s",
					cache->bic_filename));
		goto errout;
	}

	DBG(CACHE, ul_debug("reading cache file %s",
				cache->bic_filename));

	file = fdopen(fd, "r" UL_CLOEXECSTR);
	if (!file)
		goto errout;

	while (fgets(buf, sizeof(buf), file)) {
		blkid_dev dev;
		unsigned int end;

		lineno++;
		if (buf[0] == 0)
			continue;
		end = strlen(buf) - 1;
		 
		while (end < (sizeof(buf) - 2) && buf[end] == '\\' &&
		       fgets(buf + end, sizeof(buf) - end, file)) {
			end = strlen(buf) - 1;
			lineno++;
		}

		if (blkid_parse_line(cache, &dev, buf) < 0) {
			DBG(READ, ul_debug("blkid: bad format on line %d", lineno));
			continue;
		}
	}
	fclose(file);

	 
	cache->bic_flags &= ~BLKID_BIC_FL_CHANGED;
	cache->bic_ftime = st.st_mtime;

	return;
errout:
	close(fd);
	return;
}
