lazy_stat(struct archive_write_disk *a)
{
	if (a->pst != NULL) {
		 
		return (ARCHIVE_OK);
	}
#ifdef HAVE_FSTAT
	if (a->fd >= 0 && fstat(a->fd, &a->st) == 0) {
		a->pst = &a->st;
		return (ARCHIVE_OK);
	}
#endif
	 
	if (lstat(a->name, &a->st) == 0) {
		a->pst = &a->st;
		return (ARCHIVE_OK);
	}
	archive_set_error(&a->archive, errno, "Couldn't stat file");
	return (ARCHIVE_WARN);
}
