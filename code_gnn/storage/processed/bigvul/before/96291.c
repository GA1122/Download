nfs_disable_share_one(const char *sharepath, const char *host,
    const char *security, const char *access, void *cookie)
{
	int rc;
	char *linuxhost, *hostpath;
	char *argv[4];

	rc = get_linux_hostspec(host, &linuxhost);

	if (rc < 0)
		exit(1);

	hostpath = malloc(strlen(linuxhost) + 1 + strlen(sharepath) + 1);

	if (hostpath == NULL) {
		free(linuxhost);
		exit(1);
	}

	sprintf(hostpath, "%s:%s", linuxhost, sharepath);

	free(linuxhost);

#ifdef DEBUG
	fprintf(stderr, "unsharing %s\n", hostpath);
#endif

	argv[0] = "/usr/sbin/exportfs";
	argv[1] = "-u";
	argv[2] = hostpath;
	argv[3] = NULL;

	rc = libzfs_run_process(argv[0], argv, 0);

	free(hostpath);

	if (rc < 0)
		return (SA_SYSTEM_ERR);
	else
		return (SA_OK);
}
