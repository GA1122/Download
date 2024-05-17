M_fs_error_t M_fs_path_readlink_int(char **out, const char *path, M_bool last, M_fs_path_norm_t flags, M_fs_system_t sys_type)
{
	(void)path;
	(void)last;
	(void)flags;
	(void)sys_type;
	*out = NULL;
	return M_FS_ERROR_SUCCESS;
}
