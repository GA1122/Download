static bool do_init_cpuset_file(struct cgroup_mount_point *mp,
				const char *path, const char *name)
{
	char value[1024];
	char *childfile, *parentfile = NULL, *tmp;
	int ret;
	bool ok = false;

	childfile = cgroup_to_absolute_path(mp, path, name);
	if (!childfile)
		return false;

	 
	ret = cgroup_read_from_file(childfile, value, sizeof(value));
	if (ret < 0)
		goto out;
	if (value[0] != '\0' && value[0] != '\n') {
		ok = true;
		goto out;
	}

	 
	parentfile = strdup(path);
	if (!parentfile)
		goto out;

	tmp = strrchr(parentfile, '/');
	if (!tmp)
		goto out;
	if (tmp == parentfile)
		tmp++;  
	*tmp = '\0';
	tmp = parentfile;
	parentfile = cgroup_to_absolute_path(mp, tmp, name);
	free(tmp);
	if (!parentfile)
		goto out;

	 
	ret = cgroup_read_from_file(parentfile, value, sizeof(value));
	if (ret < 0)
		goto out;
	if (ret == sizeof(value)) {
		 
		ERROR("parent cpuset value too long");
		goto out;
	}
	ok = (lxc_write_to_file(childfile, value, strlen(value), false) >= 0);
	if (!ok)
		SYSERROR("failed writing %s", childfile);

out:
	free(parentfile);
	free(childfile);
	return ok;
}
