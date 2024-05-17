archive_write_new(void)
{
	struct archive_write *a;
	unsigned char *nulls;

	a = (struct archive_write *)malloc(sizeof(*a));
	if (a == NULL)
		return (NULL);
	memset(a, 0, sizeof(*a));
	a->archive.magic = ARCHIVE_WRITE_MAGIC;
	a->archive.state = ARCHIVE_STATE_NEW;
	a->archive.vtable = archive_write_vtable();
	 
	a->bytes_per_block = 10240;
	a->bytes_in_last_block = -1;	 

	 
	a->null_length = 1024;
	nulls = (unsigned char *)malloc(a->null_length);
	if (nulls == NULL) {
		free(a);
		return (NULL);
	}
	memset(nulls, 0, a->null_length);
	a->nulls = nulls;
	return (&a->archive);
}
