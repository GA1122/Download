static int cgm_set(const char *filename, const char *value, const char *name, const char *lxcpath)
{
	pid_t pid;
	int p[2], ret, v;

	if (pipe(p) < 0)
		return -1;
	if ((pid = fork()) < 0) {
		close(p[1]);
		close(p[0]);
		return -1;
	}
	if (!pid)  
		do_cgm_set(name, lxcpath, filename, value, p[1]);
	close(p[1]);
	ret = read(p[0], &v, sizeof(v));
	close(p[0]);
	if (wait_for_pid(pid))
		WARN("do_cgm_set exited with error");
	if (ret != sizeof(v) || !v)
		return -1;
	return 0;
}