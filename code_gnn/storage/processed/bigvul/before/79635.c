static M_bool M_fs_isfileintodir(const char *p1, const char *p2, char **new_p2)
{
	M_fs_info_t *info1         = NULL;
	M_fs_info_t *info2         = NULL;
	char        *bname;
	M_bool       file_info_dir = M_FALSE;

	if (M_str_isempty(p1) || M_str_isempty(p2) || new_p2 == NULL)
		return M_FALSE;

	if (M_fs_info(&info1, p1, M_FS_PATH_INFO_FLAGS_BASIC) == M_FS_ERROR_SUCCESS     &&
			M_fs_info(&info2, p2, M_FS_PATH_INFO_FLAGS_BASIC) == M_FS_ERROR_SUCCESS &&
			M_fs_info_get_type(info1) != M_FS_TYPE_DIR                              &&
			M_fs_info_get_type(info2) == M_FS_TYPE_DIR)
	{
		file_info_dir = M_TRUE;
	}
	M_fs_info_destroy(info1);
	M_fs_info_destroy(info2);

	if (!file_info_dir)
		return M_FALSE;

	bname   = M_fs_path_basename(p1, M_FS_SYSTEM_AUTO);
	*new_p2 = M_fs_path_join(p2, bname, M_FS_SYSTEM_AUTO);
	M_free(bname);

 	return M_TRUE;
 }