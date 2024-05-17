static char *try_get_abs_cgroup(const char *name, const char *lxcpath,
		const char *controller)
{
	char *cgroup = NULL;

	if (abs_cgroup_supported()) {
		 
		pid_t pid = lxc_cmd_get_init_pid(name, lxcpath);
		if (pid < 0)
			return NULL;
		if (cgmanager_get_pid_cgroup_abs_sync(NULL, cgroup_manager,
				controller, pid, &cgroup) != 0) {
			cgroup = NULL;
			NihError *nerr;
			nerr = nih_error_get();
			nih_free(nerr);
		}
		return cgroup;
	}

	 
	return lxc_cmd_get_cgroup_path(name, lxcpath, controller);
}
