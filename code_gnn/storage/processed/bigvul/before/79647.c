char *M_fs_path_join(const char *p1, const char *p2, M_fs_system_t sys_type)
{
	M_buf_t *buf;
	char     sep;

	sys_type = M_fs_path_get_system_type(sys_type);

	 
	if (M_fs_path_isabs(p2, sys_type))
		return M_strdup(p2);

	buf = M_buf_create();
	sep = M_fs_path_get_system_sep(sys_type);

	 
	if (p1 != NULL && *p1 != '\0')
		M_buf_add_str(buf, p1);
	 
	if (p1 != NULL && *p1 != '\0' && p2 != NULL && *p2 != '\0' && p1[M_str_len(p1)-1] != sep)
		M_buf_add_byte(buf, (unsigned char)sep);
	 
	if (p2 != NULL && *p2 != '\0')
		M_buf_add_str(buf, p2);

	return M_buf_finish_str(buf, NULL);
}
