set_xattrs(struct archive_write_disk *a)
{
	static int warning_done = 0;

	 
	if (archive_entry_xattr_count(a->entry) != 0 && !warning_done) {
		warning_done = 1;
		archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
		    "Cannot restore extended attributes on this system");
		return (ARCHIVE_WARN);
	}
	 
	return (ARCHIVE_OK);
}
