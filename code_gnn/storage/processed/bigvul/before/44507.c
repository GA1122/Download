static struct cgroup_process_info *lxc_cgroupfs_create(const char *name, const char *path_pattern, struct cgroup_meta_data *meta_data, const char *sub_pattern)
{
	char **cgroup_path_components = NULL;
	char **p = NULL;
	char *path_so_far = NULL;
	char **new_cgroup_paths = NULL;
	char **new_cgroup_paths_sub = NULL;
	struct cgroup_mount_point *mp;
	struct cgroup_hierarchy *h;
	struct cgroup_process_info *base_info = NULL;
	struct cgroup_process_info *info_ptr;
	int saved_errno;
	int r;
	unsigned suffix = 0;
	bool had_sub_pattern = false;
	size_t i;

	if (!is_valid_cgroup(name)) {
		ERROR("Invalid cgroup name: '%s'", name);
		errno = EINVAL;
		return NULL;
	}

	if (!strstr(path_pattern, "%n")) {
		ERROR("Invalid cgroup path pattern: '%s'; contains no %%n for specifying container name", path_pattern);
		errno = EINVAL;
		return NULL;
	}

	 
	base_info = (path_pattern[0] == '/') ?
		lxc_cgroup_process_info_get_init(meta_data) :
		lxc_cgroup_process_info_get_self(meta_data);
	if (!base_info)
		return NULL;

	new_cgroup_paths = calloc(meta_data->maximum_hierarchy + 1, sizeof(char *));
	if (!new_cgroup_paths)
		goto out_initial_error;

	new_cgroup_paths_sub = calloc(meta_data->maximum_hierarchy + 1, sizeof(char *));
	if (!new_cgroup_paths_sub)
		goto out_initial_error;

	 
	for (info_ptr = base_info; info_ptr; info_ptr = info_ptr->next) {
		h = info_ptr->hierarchy;
		mp = lxc_cgroup_find_mount_point(h, info_ptr->cgroup_path, true);
		if (!mp) {
			ERROR("Could not find writable mount point for cgroup hierarchy %d while trying to create cgroup.", h->index);
			goto out_initial_error;
		}
		info_ptr->designated_mount_point = mp;

		if (lxc_string_in_array("ns", (const char **)h->subsystems))
			continue;
		if (handle_cgroup_settings(mp, info_ptr->cgroup_path) < 0) {
			ERROR("Could not set clone_children to 1 for cpuset hierarchy in parent cgroup.");
			goto out_initial_error;
		}
	}

	 
	cgroup_path_components = lxc_normalize_path(path_pattern);
	if (!cgroup_path_components)
		goto out_initial_error;

	 
	for (p = cgroup_path_components; *p || (sub_pattern && !had_sub_pattern); p++) {
		 
		char *p_eff = *p ? *p : (char *)sub_pattern;
		bool contains_name = strstr(p_eff, "%n");
		char *current_component = NULL;
		char *current_subpath = NULL;
		char *current_entire_path = NULL;
		char *parts[3];
		size_t j = 0;
		i = 0;

		 
		if (!*p) {
			had_sub_pattern = true;
			p--;
		}

		goto find_name_on_this_level;

	cleanup_name_on_this_level:
		 
		for (j = 0, info_ptr = base_info; j < i && info_ptr; info_ptr = info_ptr->next, j++) {
			r = remove_cgroup(info_ptr->designated_mount_point, info_ptr->created_paths[info_ptr->created_paths_count - 1], false);
			if (r < 0)
				WARN("could not clean up cgroup we created when trying to create container");
			free(info_ptr->created_paths[info_ptr->created_paths_count - 1]);
			info_ptr->created_paths[--info_ptr->created_paths_count] = NULL;
		}
		if (current_component != current_subpath)
			free(current_subpath);
		if (current_component != p_eff)
			free(current_component);
		current_component = current_subpath = NULL;
		 
		++suffix;

	find_name_on_this_level:
		 
		if (contains_name && suffix > 0) {
			char *buf = calloc(strlen(name) + 32, 1);
			if (!buf)
				goto out_initial_error;
			snprintf(buf, strlen(name) + 32, "%s-%u", name, suffix);
			current_component = lxc_string_replace("%n", buf, p_eff);
			free(buf);
		} else {
			current_component = contains_name ? lxc_string_replace("%n", name, p_eff) : p_eff;
		}
		parts[0] = path_so_far;
		parts[1] = current_component;
		parts[2] = NULL;
		current_subpath = path_so_far ? lxc_string_join("/", (const char **)parts, false) : current_component;

		 
		for (i = 0, info_ptr = base_info; info_ptr; info_ptr = info_ptr->next, i++) {
			char *parts2[3];

			if (lxc_string_in_array("ns", (const char **)info_ptr->hierarchy->subsystems))
				continue;
			current_entire_path = NULL;

			parts2[0] = !strcmp(info_ptr->cgroup_path, "/") ? "" : info_ptr->cgroup_path;
			parts2[1] = current_subpath;
			parts2[2] = NULL;
			current_entire_path = lxc_string_join("/", (const char **)parts2, false);

			if (!*p) {
				 
				free(new_cgroup_paths_sub[i]);
				new_cgroup_paths_sub[i] = strdup(current_entire_path);
				if (!new_cgroup_paths_sub[i])
					goto cleanup_from_error;
			} else {
				 
				free(new_cgroup_paths[i]);
				new_cgroup_paths[i] = strdup(current_entire_path);
				if (!new_cgroup_paths[i])
					goto cleanup_from_error;
			}

			r = create_cgroup(info_ptr->designated_mount_point, current_entire_path);
			if (r < 0 && errno == EEXIST && contains_name) {
				 
				free(current_entire_path);
				current_entire_path = NULL;
				goto cleanup_name_on_this_level;
			} else if (r < 0 && errno != EEXIST) {
				SYSERROR("Could not create cgroup '%s' in '%s'.", current_entire_path, info_ptr->designated_mount_point->mount_point);
				goto cleanup_from_error;
			} else if (r == 0) {
				 
				r = lxc_grow_array((void ***)&info_ptr->created_paths, &info_ptr->created_paths_capacity, info_ptr->created_paths_count + 1, 8);
				if (r < 0)
					goto cleanup_from_error;
				if (!init_cpuset_if_needed(info_ptr->designated_mount_point, current_entire_path)) {
					ERROR("Failed to initialize cpuset for '%s' in '%s'.", current_entire_path, info_ptr->designated_mount_point->mount_point);
					goto cleanup_from_error;
				}
				info_ptr->created_paths[info_ptr->created_paths_count++] = current_entire_path;
			} else {
				 
				if (handle_cgroup_settings(info_ptr->designated_mount_point, info_ptr->cgroup_path) < 0) {
					ERROR("Could not set clone_children to 1 for cpuset hierarchy in pre-existing cgroup.");
					goto cleanup_from_error;
				}
				if (!init_cpuset_if_needed(info_ptr->designated_mount_point, info_ptr->cgroup_path)) {
					ERROR("Failed to initialize cpuset in pre-existing '%s'.", info_ptr->cgroup_path);
					goto cleanup_from_error;
				}

				 
				free(current_entire_path);
				current_entire_path = NULL;
			}
		}

		 
		free(path_so_far);
		path_so_far = strdup(current_subpath);
		if (!path_so_far)
			goto cleanup_from_error;

		 
		if (current_component != current_subpath)
			free(current_subpath);
		if (current_component != p_eff)
			free(current_component);
		current_component = current_subpath = NULL;
		continue;

	cleanup_from_error:
		 
		saved_errno = errno;
		if (current_component != current_subpath)
			free(current_subpath);
		if (current_component != p_eff)
			free(current_component);
		free(current_entire_path);
		errno = saved_errno;
		goto out_initial_error;
	}

	 
	for (i = 0, info_ptr = base_info; info_ptr; info_ptr = info_ptr->next, i++) {
		 
		if (lxc_string_in_array("ns", (const char **)info_ptr->hierarchy->subsystems))
			continue;
		free(info_ptr->cgroup_path);
		info_ptr->cgroup_path = new_cgroup_paths[i];
		info_ptr->cgroup_path_sub = new_cgroup_paths_sub[i];
	}
	 
	free(new_cgroup_paths);
	free(new_cgroup_paths_sub);
	free(path_so_far);
	lxc_free_array((void **)cgroup_path_components, free);
	return base_info;

out_initial_error:
	saved_errno = errno;
	free(path_so_far);
	lxc_cgroup_process_info_free_and_remove(base_info);
	lxc_free_array((void **)new_cgroup_paths, free);
	lxc_free_array((void **)new_cgroup_paths_sub, free);
	lxc_free_array((void **)cgroup_path_components, free);
	errno = saved_errno;
	return NULL;
}
