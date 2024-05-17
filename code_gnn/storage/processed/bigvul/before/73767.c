smb_ofile_open_check(smb_ofile_t *of, uint32_t desired_access,
    uint32_t share_access)
{
	ASSERT(of->f_magic == SMB_OFILE_MAGIC);

	mutex_enter(&of->f_mutex);

	if (of->f_state != SMB_OFILE_STATE_OPEN) {
		mutex_exit(&of->f_mutex);
		return (NT_STATUS_INVALID_HANDLE);
	}

	 
	if ((of->f_granted_access & FILE_DATA_ALL) == 0) {
		mutex_exit(&of->f_mutex);
		return (NT_STATUS_SUCCESS);
	}

	 
	if (SMB_DENY_DELETE(share_access) && (of->f_granted_access & DELETE)) {
		mutex_exit(&of->f_mutex);
		return (NT_STATUS_SHARING_VIOLATION);
	}

	if (SMB_DENY_READ(share_access) &&
	    (of->f_granted_access & (FILE_READ_DATA | FILE_EXECUTE))) {
		mutex_exit(&of->f_mutex);
		return (NT_STATUS_SHARING_VIOLATION);
	}

	if (SMB_DENY_WRITE(share_access) &&
	    (of->f_granted_access & (FILE_WRITE_DATA | FILE_APPEND_DATA))) {
		mutex_exit(&of->f_mutex);
		return (NT_STATUS_SHARING_VIOLATION);
	}

	 
	if (SMB_DENY_DELETE(of->f_share_access) && (desired_access & DELETE)) {
		mutex_exit(&of->f_mutex);
		return (NT_STATUS_SHARING_VIOLATION);
	}

	if (SMB_DENY_READ(of->f_share_access) &&
	    (desired_access & (FILE_READ_DATA | FILE_EXECUTE))) {
		mutex_exit(&of->f_mutex);
		return (NT_STATUS_SHARING_VIOLATION);
	}

	if (SMB_DENY_WRITE(of->f_share_access) &&
	    (desired_access & (FILE_WRITE_DATA | FILE_APPEND_DATA))) {
		mutex_exit(&of->f_mutex);
		return (NT_STATUS_SHARING_VIOLATION);
	}

	mutex_exit(&of->f_mutex);
	return (NT_STATUS_SUCCESS);
}