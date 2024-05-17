M_fs_error_t M_fs_path_readlink_int(char **out, const char *path, M_bool last, M_fs_path_norm_t flags, M_fs_system_t sys_type)
{
	char        *temp;
	ssize_t      read_len;
	size_t       path_max;
	int          errsv;
	M_fs_info_t *info;

	if (out == NULL || path == NULL) {
		return M_FS_ERROR_INVALID;
	}
	*out = NULL;

	 
	if (M_fs_info(&info, path, M_FS_PATH_INFO_FLAGS_BASIC) != M_FS_ERROR_SUCCESS) {
		 
		return M_FS_ERROR_SUCCESS;
	}
	if (M_fs_info_get_type(info) != M_FS_TYPE_SYMLINK) {
		 
		M_fs_info_destroy(info);
		return M_FS_ERROR_SUCCESS;
	}
	M_fs_info_destroy(info);

	path_max = M_fs_path_get_path_max(sys_type);
	temp     = M_malloc_zero(path_max);

	 
	read_len = readlink(path, temp, path_max-1);
	if (read_len == -1) {
		errsv = errno;
		M_free(temp);
		 
		if (errsv == EINVAL) {
			return M_FS_ERROR_SUCCESS;
		}
		 
		if (errsv == ENOENT) {
			if ((flags & M_FS_PATH_NORM_SYMLINKS_FAILDNE && !last) || (flags & M_FS_PATH_NORM_SYMLINKS_FAILDNELAST && last)) {
				return M_FS_ERROR_DNE;
			} else {
				return M_FS_ERROR_SUCCESS;
			}
		}
		return M_fs_error_from_syserr(errsv);
	} else {
		 
		temp[read_len] = '\0';
	}
	 
	*out = M_strdup(temp);
	M_free(temp);
	return M_FS_ERROR_SUCCESS;
}
