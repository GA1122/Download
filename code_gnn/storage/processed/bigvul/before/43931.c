set_fflags_platform(struct archive_write_disk *a, int fd, const char *name,
    mode_t mode, unsigned long set, unsigned long clear)
{
	(void)a;  
	(void)fd;  
	(void)name;  
	(void)mode;  
	(void)set;  
	(void)clear;  
	return (ARCHIVE_OK);
}
