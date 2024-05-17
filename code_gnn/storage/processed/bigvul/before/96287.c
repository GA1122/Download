nfs_available(void)
{
	if (nfs_exportfs_temp_fd == -1)
		(void) nfs_check_exportfs();

	return ((nfs_exportfs_temp_fd != -1) ? B_TRUE : B_FALSE);
}
