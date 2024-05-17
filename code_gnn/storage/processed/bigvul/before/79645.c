M_bool M_fs_path_ishidden(const char *path, M_fs_info_t *info)
{
	M_bool have_info;
	M_bool ret;

	if ((path == NULL || *path == '\0') && info == NULL) {
		return M_FALSE;
	}

	have_info = (info == NULL) ? M_FALSE : M_TRUE;
	if (!have_info) {
		if (M_fs_info(&info, path, M_FS_PATH_INFO_FLAGS_BASIC) != M_FS_ERROR_SUCCESS) {
			return M_FALSE;
		}
	}

	ret = M_fs_info_get_ishidden(info);

	if (!have_info) {
		M_fs_info_destroy(info);
	}

	return ret;
}
