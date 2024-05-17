NTSTATUS fd_close(files_struct *fsp)
{
	int ret;

	if (fsp->dptr) {
		dptr_CloseDir(fsp);
	}
	if (fsp->fh->fd == -1) {
		return NT_STATUS_OK;  
	}
	if (fsp->fh->ref_count > 1) {
		return NT_STATUS_OK;  
	}

	ret = SMB_VFS_CLOSE(fsp);
	fsp->fh->fd = -1;
	if (ret == -1) {
		return map_nt_error_from_unix(errno);
	}
	return NT_STATUS_OK;
}
