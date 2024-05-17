static M_fs_error_t M_fs_delete_file(const char *path)
{
	if (!DeleteFile(path)) {
		return M_fs_error_from_syserr(GetLastError());
	}
	return M_FS_ERROR_SUCCESS;
}
