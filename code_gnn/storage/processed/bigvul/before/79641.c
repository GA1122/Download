size_t M_fs_path_get_path_max(M_fs_system_t sys_type)
{
	long path_max = 0;

	 
	sys_type = M_fs_path_get_system_type(sys_type);

	 
#ifdef PATH_MAX
	path_max = PATH_MAX;
#elif !defined(_WIN32)
	path_max = pathconf("/", _PC_PATH_MAX);
#endif

	 
	if (path_max <= 0 || path_max > 65536) {
		if (sys_type == M_FS_SYSTEM_WINDOWS) {
			path_max = 260;
		} else {
			path_max = 4096;
		}
	}

	return (size_t)path_max;
}
