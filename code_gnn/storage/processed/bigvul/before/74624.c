int blkid_flush_cache(blkid_cache cache)
{
	struct list_head *p;
	char *tmp = NULL;
	char *opened = NULL;
	char *filename;
	FILE *file = NULL;
	int fd, ret = 0;
	struct stat st;

	if (!cache)
		return -BLKID_ERR_PARAM;

	if (list_empty(&cache->bic_devs) ||
	    !(cache->bic_flags & BLKID_BIC_FL_CHANGED)) {
		DBG(SAVE, ul_debug("skipping cache file write"));
		return 0;
	}

	filename = cache->bic_filename ? cache->bic_filename :
					 blkid_get_cache_filename(NULL);
	if (!filename)
		return -BLKID_ERR_PARAM;

	if (strncmp(filename,
	    BLKID_RUNTIME_DIR "/", sizeof(BLKID_RUNTIME_DIR)) == 0) {

		 
		if (stat(BLKID_RUNTIME_DIR, &st)
		    && errno == ENOENT
		    && mkdir(BLKID_RUNTIME_DIR, S_IWUSR|
						S_IRUSR|S_IRGRP|S_IROTH|
						S_IXUSR|S_IXGRP|S_IXOTH) != 0
		    && errno != EEXIST) {
			DBG(SAVE, ul_debug("can't create %s directory for cache file",
					BLKID_RUNTIME_DIR));
			return 0;
		}
	}

	 
	if (((ret = stat(filename, &st)) < 0 && errno != ENOENT) ||
	    (ret == 0 && access(filename, W_OK) < 0)) {
		DBG(SAVE, ul_debug("can't write to cache file %s", filename));
		return 0;
	}

	 
	if (ret == 0 && S_ISREG(st.st_mode)) {
		tmp = malloc(strlen(filename) + 8);
		if (tmp) {
			sprintf(tmp, "%s-XXXXXX", filename);
			fd = mkostemp(tmp, O_RDWR|O_CREAT|O_EXCL|O_CLOEXEC);
			if (fd >= 0) {
				if (fchmod(fd, 0644) != 0)
					DBG(SAVE, ul_debug("%s: fchmod failed", filename));
				else if ((file = fdopen(fd, "w" UL_CLOEXECSTR)))
					opened = tmp;
				if (!file)
					close(fd);
			}
		}
	}

	if (!file) {
		file = fopen(filename, "w" UL_CLOEXECSTR);
		opened = filename;
	}

	DBG(SAVE, ul_debug("writing cache file %s (really %s)",
		   filename, opened));

	if (!file) {
		ret = errno;
		goto errout;
	}

	list_for_each(p, &cache->bic_devs) {
		blkid_dev dev = list_entry(p, struct blkid_struct_dev, bid_devs);
		if (!dev->bid_type || (dev->bid_flags & BLKID_BID_FL_REMOVABLE))
			continue;
		if ((ret = save_dev(dev, file)) < 0)
			break;
	}

	if (ret >= 0) {
		cache->bic_flags &= ~BLKID_BIC_FL_CHANGED;
		ret = 1;
	}

	if (close_stream(file) != 0)
		DBG(SAVE, ul_debug("write failed: %s", filename));

	if (opened != filename) {
		if (ret < 0) {
			unlink(opened);
			DBG(SAVE, ul_debug("unlinked temp cache %s", opened));
		} else {
			char *backup;

			backup = malloc(strlen(filename) + 5);
			if (backup) {
				sprintf(backup, "%s.old", filename);
				unlink(backup);
				if (link(filename, backup)) {
					DBG(SAVE, ul_debug("can't link %s to %s",
							filename, backup));
				}
				free(backup);
			}
			if (rename(opened, filename)) {
				ret = errno;
				DBG(SAVE, ul_debug("can't rename %s to %s",
						opened, filename));
			} else {
				DBG(SAVE, ul_debug("moved temp cache %s", opened));
			}
		}
	}

errout:
	free(tmp);
	if (filename != cache->bic_filename)
		free(filename);
	return ret;
}