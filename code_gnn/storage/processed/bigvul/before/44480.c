static bool find_hierarchy_mountpts( struct cgroup_meta_data *meta_data, char **kernel_subsystems)
{
	bool bret = false;
	FILE *proc_self_mountinfo;
	char *line = NULL;
	size_t sz = 0;
	char **tokens = NULL;
	size_t mount_point_count = 0;
	size_t mount_point_capacity = 0;
	size_t token_capacity = 0;
	int r;

	proc_self_mountinfo = fopen_cloexec("/proc/self/mountinfo", "r");
	 
	if (!proc_self_mountinfo)
		proc_self_mountinfo = fopen_cloexec("/proc/1/mountinfo", "r");
	if (!proc_self_mountinfo)
		return false;

	while (getline(&line, &sz, proc_self_mountinfo) != -1) {
		char *token, *line_tok, *saveptr = NULL;
		size_t i, j, k;
		struct cgroup_mount_point *mount_point;
		struct cgroup_hierarchy *h;
		char **subsystems;

		if (line[0] && line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';

		for (i = 0, line_tok = line; (token = strtok_r(line_tok, " ", &saveptr)); line_tok = NULL) {
			r = lxc_grow_array((void ***)&tokens, &token_capacity, i + 1, 64);
			if (r < 0)
				goto out;
			tokens[i++] = token;
		}

		 
		for (j = 6; j < i && tokens[j]; j++)
			if (!strcmp(tokens[j], "-"))
				break;

		 
		if (j >= i || !tokens[j])
			continue;
		 
		if (i != j + 4)
			continue;

		 
		if (strcmp(tokens[j + 1], "cgroup") != 0)
			continue;

		subsystems = subsystems_from_mount_options(tokens[j + 3], kernel_subsystems);
		if (!subsystems)
			goto out;

		h = NULL;
		for (k = 1; k <= meta_data->maximum_hierarchy; k++) {
			if (meta_data->hierarchies[k] &&
			    meta_data->hierarchies[k]->subsystems[0] &&
			    lxc_string_in_array(meta_data->hierarchies[k]->subsystems[0], (const char **)subsystems)) {
				 
				h = meta_data->hierarchies[k];
				break;
			}
		}
		lxc_free_array((void **)subsystems, free);

		r = lxc_grow_array((void ***)&meta_data->mount_points, &mount_point_capacity, mount_point_count + 1, 12);
		if (r < 0)
			goto out;

		 
		mount_point = calloc(1, sizeof(*mount_point));
		if (!mount_point)
			goto out;

		meta_data->mount_points[mount_point_count++] = mount_point;

		mount_point->hierarchy = h;
		mount_point->mount_point = strdup(tokens[4]);
		mount_point->mount_prefix = strdup(tokens[3]);
		if (!mount_point->mount_point || !mount_point->mount_prefix)
			goto out;
		mount_point->read_only = !lxc_string_in_list("rw", tokens[5], ',');

		if (!strcmp(mount_point->mount_prefix, "/")) {
			if (mount_point->read_only) {
				if (!h->ro_absolute_mount_point)
					h->ro_absolute_mount_point = mount_point;
			} else {
				if (!h->rw_absolute_mount_point)
					h->rw_absolute_mount_point = mount_point;
			}
		}

		k = lxc_array_len((void **)h->all_mount_points);
		r = lxc_grow_array((void ***)&h->all_mount_points, &h->all_mount_point_capacity, k + 1, 4);
		if (r < 0)
			goto out;
		h->all_mount_points[k] = mount_point;
	}
	bret = true;

out:
	fclose(proc_self_mountinfo);
	free(tokens);
	free(line);
	return bret;
}
