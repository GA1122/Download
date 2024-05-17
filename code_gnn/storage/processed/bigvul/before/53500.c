archive_read_format_mtree_options(struct archive_read *a,
    const char *key, const char *val)
{
	struct mtree *mtree;

	mtree = (struct mtree *)(a->format->data);
	if (strcmp(key, "checkfs")  == 0) {
		 
		if (val == NULL || val[0] == 0) {
			mtree->checkfs = 0;
		} else {
			mtree->checkfs = 1;
		}
		return (ARCHIVE_OK);
	}

	 
	return (ARCHIVE_WARN);
}
