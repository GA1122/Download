static void lxc_attach_get_init_uidgid(uid_t* init_uid, gid_t* init_gid)
{
	FILE *proc_file;
	char proc_fn[MAXPATHLEN];
	char *line = NULL;
	size_t line_bufsz = 0;
	int ret;
	long value = -1;
	uid_t uid = (uid_t)-1;
	gid_t gid = (gid_t)-1;

	 
	snprintf(proc_fn, MAXPATHLEN, "/proc/%d/status", 1);

	proc_file = fopen(proc_fn, "r");
	if (!proc_file)
		return;

	while (getline(&line, &line_bufsz, proc_file) != -1) {
		 
		ret = sscanf(line, "Uid: %ld", &value);
		if (ret != EOF && ret > 0) {
			uid = (uid_t) value;
		} else {
			ret = sscanf(line, "Gid: %ld", &value);
			if (ret != EOF && ret > 0)
				gid = (gid_t) value;
		}
		if (uid != (uid_t)-1 && gid != (gid_t)-1)
			break;
	}

	fclose(proc_file);
	free(line);

	 
	if (uid != (uid_t)-1)
		*init_uid = uid;
	if (gid != (gid_t)-1)
		*init_gid = gid;

	 
}