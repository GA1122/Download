create_parent_dir(struct archive_write_disk *a, char *path)
{
	char *slash;
	int r;

	 
	slash = strrchr(path, '/');
	if (slash == NULL)
		return (ARCHIVE_OK);
	*slash = '\0';
	r = create_dir(a, path);
	*slash = '/';
	return (r);
}
