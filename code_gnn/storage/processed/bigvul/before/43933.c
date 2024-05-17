set_mac_metadata(struct archive_write_disk *a, const char *pathname,
		 const void *metadata, size_t metadata_size)
{
	struct archive_string tmp;
	ssize_t written;
	int fd;
	int ret = ARCHIVE_OK;

	 
	archive_string_init(&tmp);
	archive_strcpy(&tmp, pathname);
	archive_strcat(&tmp, ".XXXXXX");
	fd = mkstemp(tmp.s);

	if (fd < 0) {
		archive_set_error(&a->archive, errno,
				  "Failed to restore metadata");
		archive_string_free(&tmp);
		return (ARCHIVE_WARN);
	}
	written = write(fd, metadata, metadata_size);
	close(fd);
	if ((size_t)written != metadata_size) {
		archive_set_error(&a->archive, errno,
				  "Failed to restore metadata");
		ret = ARCHIVE_WARN;
	} else {
		int compressed;

#if defined(UF_COMPRESSED)
		if ((a->todo & TODO_HFS_COMPRESSION) != 0 &&
		    (ret = lazy_stat(a)) == ARCHIVE_OK)
			compressed = a->st.st_flags & UF_COMPRESSED;
		else
#endif
			compressed = 0;
		ret = copy_metadata(a, tmp.s, pathname, compressed);
	}
	unlink(tmp.s);
	archive_string_free(&tmp);
	return (ret);
}
