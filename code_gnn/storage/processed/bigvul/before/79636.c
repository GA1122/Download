static M_fs_error_t M_fs_move_file(const char *path_old, const char *path_new)
{
	M_fs_error_t  res;

	 
#ifdef _WIN32
	if (MoveFileEx(path_old, path_new, MOVEFILE_REPLACE_EXISTING))
#else
	if (rename(path_old, path_new) == 0)
#endif
	{
		res = M_FS_ERROR_SUCCESS;
	} else {
#ifdef _WIN32
		res = M_fs_error_from_syserr(GetLastError());
#else
		res = M_fs_error_from_syserr(errno);
#endif
	}

	return res;
}