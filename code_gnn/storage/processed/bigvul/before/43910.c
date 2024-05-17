edit_deep_directories(struct archive_write_disk *a)
{
	int ret;
	char *tail = a->name;

	 
	if (strlen(tail) <= PATH_MAX)
		return;

	 
	a->restore_pwd = open(".", O_RDONLY | O_BINARY | O_CLOEXEC);
	__archive_ensure_cloexec_flag(a->restore_pwd);
	if (a->restore_pwd < 0)
		return;

	 
	while (strlen(tail) > PATH_MAX) {
		 
		tail += PATH_MAX - 8;
		while (tail > a->name && *tail != '/')
			tail--;
		 
		if (tail <= a->name)
			return;
		 
		*tail = '\0';  
		ret = create_dir(a, a->name);
		if (ret == ARCHIVE_OK && chdir(a->name) != 0)
			ret = ARCHIVE_FAILED;
		*tail = '/';  
		if (ret != ARCHIVE_OK)
			return;
		tail++;
		 
		a->name = tail;
	}
	return;
}
