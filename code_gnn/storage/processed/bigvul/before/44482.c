static int handle_cgroup_settings(struct cgroup_mount_point *mp,
				  char *cgroup_path)
{
	int r, saved_errno = 0;
	char buf[2];

	mp->need_cpuset_init = false;

	 
	if (lxc_string_in_array("memory", (const char **)mp->hierarchy->subsystems)) {
		char *cc_path = cgroup_to_absolute_path(mp, cgroup_path, "/memory.use_hierarchy");
		if (cc_path) {
			r = lxc_read_from_file(cc_path, buf, 1);
			if (r < 1 || buf[0] != '1') {
				r = lxc_write_to_file(cc_path, "1", 1, false);
				if (r < 0)
					SYSERROR("failed to set memory.use_hierarchy to 1; continuing");
			}
			free(cc_path);
		}
	}

	 
	if (lxc_string_in_array("cpuset", (const char **)mp->hierarchy->subsystems)) {
		char *cc_path = cgroup_to_absolute_path(mp, cgroup_path, "/cgroup.clone_children");
		struct stat sb;

		if (!cc_path)
			return -1;
		 
		if (stat(cc_path, &sb) != 0 && errno == ENOENT) {
			mp->need_cpuset_init = true;
			free(cc_path);
			return 0;
		}
		r = lxc_read_from_file(cc_path, buf, 1);
		if (r == 1 && buf[0] == '1') {
			free(cc_path);
			return 0;
		}
		r = lxc_write_to_file(cc_path, "1", 1, false);
		saved_errno = errno;
		free(cc_path);
		errno = saved_errno;
		return r < 0 ? -1 : 0;
	}
	return 0;
}
