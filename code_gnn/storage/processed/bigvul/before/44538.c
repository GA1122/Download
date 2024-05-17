static bool collect_subsytems(void)
{
	char *line = NULL;
	nih_local char **cgm_subsys_list = NULL;
	size_t sz = 0;
	FILE *f = NULL;

	if (subsystems)  
		return true;

	subsystems_inone = malloc(2 * sizeof(char *));
	if (!subsystems_inone)
		return false;
	subsystems_inone[0] = "all";
	subsystems_inone[1] = NULL;

	if (lxc_list_controllers(&cgm_subsys_list)) {
		while (cgm_subsys_list[nr_subsystems]) {
			char **tmp = NIH_MUST( realloc(subsystems,
						(nr_subsystems+2)*sizeof(char *)) );
			tmp[nr_subsystems] = NIH_MUST(
					strdup(cgm_subsys_list[nr_subsystems++]) );
			subsystems = tmp;
		}
		if (nr_subsystems)
			subsystems[nr_subsystems] = NULL;
		goto collected;
	}

	INFO("cgmanager_list_controllers failed, falling back to /proc/self/cgroups");
	f = fopen_cloexec("/proc/self/cgroup", "r");
	if (!f) {
		f = fopen_cloexec("/proc/1/cgroup", "r");
		if (!f)
			return false;
	}
	while (getline(&line, &sz, f) != -1) {
		 
		char *slist, *end, *p, *saveptr = NULL, **tmp;

		if (!line[0])
			continue;

		slist = strchr(line, ':');
		if (!slist)
			continue;
		slist++;
		end = strchr(slist, ':');
		if (!end)
			continue;
		*end = '\0';

		for (p = strtok_r(slist, ",", &saveptr);
				p;
				p = strtok_r(NULL, ",", &saveptr)) {
			tmp = realloc(subsystems, (nr_subsystems+2)*sizeof(char *));
			if (!tmp)
				goto out_free;

			subsystems = tmp;
			tmp[nr_subsystems] = strdup(p);
			tmp[nr_subsystems+1] = NULL;
			if (!tmp[nr_subsystems])
				goto out_free;
			nr_subsystems++;
		}
	}
	fclose(f);
	f = NULL;

	free(line);
	line = NULL;

collected:
	if (!nr_subsystems) {
		ERROR("No cgroup subsystems found");
		return false;
	}

	 
	const char *cgroup_use = lxc_global_config_value("lxc.cgroup.use");
	if (!cgroup_use && errno != 0)
		goto out_good;
	if (cgroup_use) {
		if (!verify_and_prune(cgroup_use)) {
			free_subsystems();
			return false;
		}
		subsystems_inone[0] = NIH_MUST( strdup(cgroup_use) );
		cgm_all_controllers_same = false;
	}

out_good:
	return true;

out_free:
	free(line);
	if (f)
		fclose(f);
	free_subsystems();
	return false;
}