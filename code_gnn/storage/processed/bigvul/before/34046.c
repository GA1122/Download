__archive_write_filter(struct archive_write_filter *f,
    const void *buff, size_t length)
{
	int r;
	if (length == 0)
		return(ARCHIVE_OK);
	if (f->write == NULL)
		 
		return(ARCHIVE_FATAL);
	r = (f->write)(f, buff, length);
	f->bytes_written += length;
	return (r);
}
