archive_write_disk_new(void)
{
	struct archive_write_disk *a;

	a = (struct archive_write_disk *)malloc(sizeof(*a));
	if (a == NULL)
		return (NULL);
	memset(a, 0, sizeof(*a));
	a->archive.magic = ARCHIVE_WRITE_DISK_MAGIC;
	 
	a->archive.state = ARCHIVE_STATE_HEADER;
	a->archive.vtable = archive_write_disk_vtable();
	a->start_time = time(NULL);
	 
	umask(a->user_umask = umask(0));
#ifdef HAVE_GETEUID
	a->user_uid = geteuid();
#endif  
	if (archive_string_ensure(&a->path_safe, 512) == NULL) {
		free(a);
		return (NULL);
	}
#ifdef HAVE_ZLIB_H
	a->decmpfs_compression_level = 5;
#endif
	return (&a->archive);
}
