entry_to_archive(struct cpio *cpio, struct archive_entry *entry)
{
	const char *destpath = archive_entry_pathname(entry);
	const char *srcpath = archive_entry_sourcepath(entry);
	int fd = -1;
	ssize_t bytes_read;
	int r;

	 
	if (cpio->verbose)
		fprintf(stderr,"%s", destpath);
	if (cpio->dot)
		fprintf(stderr, ".");

	 
	if (cpio->option_link
	    && archive_entry_filetype(entry) == AE_IFREG)
	{
		struct archive_entry *t;
		 
		t = archive_entry_clone(entry);
		if (t == NULL)
			lafe_errc(1, ENOMEM, "Can't create link");
		 
		archive_entry_set_hardlink(t, srcpath);
		 
		archive_entry_set_size(t, 0);
		r = archive_write_header(cpio->archive, t);
		archive_entry_free(t);
		if (r != ARCHIVE_OK)
			lafe_warnc(archive_errno(cpio->archive),
			    "%s", archive_error_string(cpio->archive));
		if (r == ARCHIVE_FATAL)
			exit(1);
#ifdef EXDEV
		if (r != ARCHIVE_OK && archive_errno(cpio->archive) == EXDEV) {
			 
			lafe_warnc(0, "Copying file instead");
		} else
#endif
		return (0);
	}

	 
	if (archive_entry_filetype(entry) == AE_IFREG) {
		if (archive_entry_size(entry) > 0) {
			fd = open(srcpath, O_RDONLY | O_BINARY);
			if (fd < 0) {
				lafe_warnc(errno,
				    "%s: could not open file", srcpath);
				goto cleanup;
			}
		}
	} else {
		archive_entry_set_size(entry, 0);
	}

	r = archive_write_header(cpio->archive, entry);

	if (r != ARCHIVE_OK)
		lafe_warnc(archive_errno(cpio->archive),
		    "%s: %s",
		    srcpath,
		    archive_error_string(cpio->archive));

	if (r == ARCHIVE_FATAL)
		exit(1);

	if (r >= ARCHIVE_WARN && archive_entry_size(entry) > 0 && fd >= 0) {
		bytes_read = read(fd, cpio->buff, (unsigned)cpio->buff_size);
		while (bytes_read > 0) {
			ssize_t bytes_write;
			bytes_write = archive_write_data(cpio->archive,
			    cpio->buff, bytes_read);
			if (bytes_write < 0)
				lafe_errc(1, archive_errno(cpio->archive),
				    "%s", archive_error_string(cpio->archive));
			if (bytes_write < bytes_read) {
				lafe_warnc(0,
				    "Truncated write; file may have "
				    "grown while being archived.");
			}
			bytes_read = read(fd, cpio->buff,
			    (unsigned)cpio->buff_size);
		}
	}

	fd = restore_time(cpio, entry, srcpath, fd);

cleanup:
	if (cpio->verbose)
		fprintf(stderr,"\n");
	if (fd >= 0)
		close(fd);
	return (0);
}
