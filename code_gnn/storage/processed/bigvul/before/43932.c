set_mac_metadata(struct archive_write_disk *a, const char *pathname,
		 const void *metadata, size_t metadata_size)
{
	(void)a;  
	(void)pathname;  
	(void)metadata;  
	(void)metadata_size;  
	return (ARCHIVE_OK);
}
