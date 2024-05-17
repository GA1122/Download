static char *cgroup_to_absolute_path(struct cgroup_mount_point *mp,
				     const char *path, const char *suffix)
{
	 
	char *prefix = mp->mount_prefix;
	char *buf;
	ssize_t len, rv;

	 
	if (path[0] != '/') {
		errno = EINVAL;
		return NULL;
	}

	if (prefix && !strcmp(prefix, "/"))
		prefix = NULL;

	 
	if (prefix && strncmp(prefix, path, strlen(prefix)) != 0) {
		errno = EINVAL;
		return NULL;
	}
	 
	if (prefix && path[strlen(prefix)] != '/' && path[strlen(prefix)] != '\0') {
		errno = EINVAL;
		return NULL;
	}

	 
	path += prefix ? strlen(prefix) : 0;

	len = strlen(mp->mount_point) + strlen(path) + (suffix ? strlen(suffix) : 0);
	buf = calloc(len + 1, 1);
	if (!buf)
		return NULL;
	rv = snprintf(buf, len + 1, "%s%s%s", mp->mount_point, path, suffix ? suffix : "");
	if (rv > len) {
		free(buf);
		errno = ENOMEM;
		return NULL;
	}

	return buf;
}
