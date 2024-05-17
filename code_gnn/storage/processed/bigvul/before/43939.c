set_xattrs(struct archive_write_disk *a)
{
	struct archive_entry *entry = a->entry;
	static int warning_done = 0;
	int ret = ARCHIVE_OK;
	int i = archive_entry_xattr_reset(entry);

	while (i--) {
		const char *name;
		const void *value;
		size_t size;
		archive_entry_xattr_next(entry, &name, &value, &size);
		if (name != NULL &&
				strncmp(name, "xfsroot.", 8) != 0 &&
				strncmp(name, "system.", 7) != 0) {
			int e;
#if HAVE_FSETXATTR
			if (a->fd >= 0)
				e = fsetxattr(a->fd, name, value, size, 0);
			else
#elif HAVE_FSETEA
			if (a->fd >= 0)
				e = fsetea(a->fd, name, value, size, 0);
			else
#endif
			{
#if HAVE_LSETXATTR
				e = lsetxattr(archive_entry_pathname(entry),
				    name, value, size, 0);
#elif HAVE_LSETEA
				e = lsetea(archive_entry_pathname(entry),
				    name, value, size, 0);
#endif
			}
			if (e == -1) {
				if (errno == ENOTSUP || errno == ENOSYS) {
					if (!warning_done) {
						warning_done = 1;
						archive_set_error(&a->archive, errno,
						    "Cannot restore extended "
						    "attributes on this file "
						    "system");
					}
				} else
					archive_set_error(&a->archive, errno,
					    "Failed to set extended attribute");
				ret = ARCHIVE_WARN;
			}
		} else {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
			    "Invalid extended attribute encountered");
			ret = ARCHIVE_WARN;
		}
	}
	return (ret);
}
