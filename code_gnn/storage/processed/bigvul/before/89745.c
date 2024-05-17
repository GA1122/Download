void fslib_copy_dir(const char *full_path) {
	assert(full_path);
	if (arg_debug || arg_debug_private_lib)
		printf("    fslib_copy_dir %s\n", full_path);

	struct stat s;
	if (stat(full_path, &s) != 0 || s.st_uid != 0 || !S_ISDIR(s.st_mode) || access(full_path, R_OK))
		return;

	char *dir_name = strrchr(full_path, '/');
	assert(dir_name);
	dir_name++;
	assert(*dir_name != '\0');

	char *dest;
	if (asprintf(&dest, "%s/%s", build_dest_dir(full_path), dir_name) == -1)
		errExit("asprintf");
	if (stat(dest, &s) == 0) {
		free(dest);
		return;
	}

	mkdir_attr(dest, 0755, 0, 0);

	if (mount(full_path, dest, NULL, MS_BIND|MS_REC, NULL) < 0 ||
		mount(NULL, dest, NULL, MS_BIND|MS_REMOUNT|MS_NOSUID|MS_NODEV|MS_REC, NULL) < 0)
		errExit("mount bind");
	fs_logger2("clone", full_path);
	fs_logger2("mount", full_path);
	dir_cnt++;
	free(dest);
}