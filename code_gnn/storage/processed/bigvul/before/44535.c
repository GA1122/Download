static bool chown_cgroup(const char *cgroup_path, struct lxc_conf *conf)
{
	struct chown_data data;
	char **slist = subsystems;
	int i;

	if (lxc_list_empty(&conf->id_map))
		 
		return true;

	data.cgroup_path = cgroup_path;
	data.origuid = geteuid();

	 
	if (userns_exec_1(conf, chown_cgroup_wrapper, &data) < 0) {
		ERROR("Error requesting cgroup chown in new namespace");
		return false;
	}

	 
	if (cgm_supports_multiple_controllers)
		slist = subsystems_inone;

	for (i = 0; slist[i]; i++) {
		if (!lxc_cgmanager_chmod(slist[i], cgroup_path, "", 0775))
			return false;
		if (!lxc_cgmanager_chmod(slist[i], cgroup_path, "tasks", 0775))
			return false;
		if (!lxc_cgmanager_chmod(slist[i], cgroup_path, "cgroup.procs", 0775))
			return false;
	}

	return true;
}
