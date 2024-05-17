static bool find_cgroup_hierarchies(struct cgroup_meta_data *meta_data,
	bool all_kernel_subsystems, bool all_named_subsystems,
	const char **subsystem_whitelist)
{
	FILE *proc_self_cgroup;
	char *line = NULL;
	size_t sz = 0;
	int r;
	bool bret = false;
	size_t hierarchy_capacity = 0;

	proc_self_cgroup = fopen_cloexec("/proc/self/cgroup", "r");
	 
	if (!proc_self_cgroup)
		proc_self_cgroup = fopen_cloexec("/proc/1/cgroup", "r");
	if (!proc_self_cgroup)
		return false;

	while (getline(&line, &sz, proc_self_cgroup) != -1) {
		 
		char *colon1;
		char *colon2;
		int hierarchy_number;
		struct cgroup_hierarchy *h = NULL;
		char **p;

		if (!line[0])
			continue;

		colon1 = strchr(line, ':');
		if (!colon1)
			continue;
		*colon1++ = '\0';
		colon2 = strchr(colon1, ':');
		if (!colon2)
			continue;
		*colon2 = '\0';

		colon2 = NULL;
		hierarchy_number = strtoul(line, &colon2, 10);
		if (!colon2 || *colon2)
			continue;

		if (hierarchy_number > meta_data->maximum_hierarchy) {
			 
			r = lxc_grow_array((void ***)&meta_data->hierarchies, &hierarchy_capacity, hierarchy_number + 1, 12);
			if (r < 0)
				goto out;

			meta_data->maximum_hierarchy = hierarchy_number;
		}

		 
		if (meta_data->hierarchies[hierarchy_number])
			goto out;

		h = calloc(1, sizeof(struct cgroup_hierarchy));
		if (!h)
			goto out;

		meta_data->hierarchies[hierarchy_number] = h;

		h->index = hierarchy_number;
		h->subsystems = lxc_string_split_and_trim(colon1, ',');
		if (!h->subsystems)
			goto out;
		 
		if (!all_kernel_subsystems || !all_named_subsystems) {
			for (p = h->subsystems; *p; p++) {
				if (!strncmp(*p, "name=", 5)) {
					if (all_named_subsystems || (subsystem_whitelist && lxc_string_in_array(*p, subsystem_whitelist))) {
						h->used = true;
						break;
					}
				} else {
					if (all_kernel_subsystems || (subsystem_whitelist && lxc_string_in_array(*p, subsystem_whitelist))) {
						h->used = true;
						break;
					}
				}
			}
		} else {
			 
			h->used = true;
		}
	}
	bret = true;

out:
	fclose(proc_self_cgroup);
	free(line);
	return bret;
}
