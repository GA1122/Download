static inline bool cgm_create(void *hdata)
{
	struct cgm_data *d = hdata;
	char **slist = subsystems;
	int i, index=0, baselen, ret;
	int32_t existed;
	char result[MAXPATHLEN], *tmp, *cgroup_path;

	if (!d)
		return false;

	memset(result, 0, MAXPATHLEN);
	tmp = lxc_string_replace("%n", d->name, d->cgroup_pattern);
	if (!tmp)
		goto bad;
	if (strlen(tmp) >= MAXPATHLEN) {
		free(tmp);
		goto bad;
	}
	strcpy(result, tmp);
	baselen = strlen(result);
	free(tmp);
	tmp = result;
	while (*tmp == '/')
		tmp++;
again:
	if (index == 100) {  
		ERROR("cgroup error?  100 cgroups with this name already running");
		goto bad;
	}
	if (index) {
		ret = snprintf(result+baselen, MAXPATHLEN-baselen, "-%d", index);
		if (ret < 0 || ret >= MAXPATHLEN-baselen)
			goto bad;
	}
	existed = 0;

	if (cgm_supports_multiple_controllers)
		slist = subsystems_inone;

	for (i = 0; slist[i]; i++) {
		if (!lxc_cgmanager_create(slist[i], tmp, &existed)) {
			ERROR("Error creating cgroup %s:%s", slist[i], result);
			cleanup_cgroups(tmp);
			goto bad;
		}
		if (existed == 1)
			goto next;
	}
	cgroup_path = strdup(tmp);
	if (!cgroup_path) {
		cleanup_cgroups(tmp);
		goto bad;
	}
	d->cgroup_path = cgroup_path;
	cgm_dbus_disconnect();
	return true;

next:
	index++;
	goto again;
bad:
	cgm_dbus_disconnect();
	return false;
}