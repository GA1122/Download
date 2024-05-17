static int cgm_get(const char *filename, char *value, size_t len, const char *name, const char *lxcpath)
{
	pid_t pid;
	int p[2], ret, newlen, readlen;

	if (pipe(p) < 0)
		return -1;
	if ((pid = fork()) < 0) {
		close(p[0]);
		close(p[1]);
		return -1;
	}
	if (!pid)  
		do_cgm_get(name, lxcpath, filename, p[1], len && value);
	close(p[1]);
	ret = read(p[0], &newlen, sizeof(newlen));
	if (ret != sizeof(newlen)) {
		close(p[0]);
		ret = -1;
		goto out;
	}
	if (!len || !value) {
		close(p[0]);
		ret = newlen;
		goto out;
	}
	memset(value, 0, len);
	if (newlen < 0) {  
		close(p[0]);
		ret = -1;
		goto out;
	}
	if (newlen == 0) {  
		close(p[0]);
		ret = 0;
		goto out;
	}
	readlen = newlen > len ? len : newlen;
	ret = read(p[0], value, readlen);
	close(p[0]);
	if (ret != readlen) {
		ret = -1;
		goto out;
	}
	if (newlen >= len) {
		value[len-1] = '\0';
		newlen = len-1;
	} else if (newlen+1 < len) {
		value[newlen++] = '\n';
		value[newlen] = '\0';
	}
	ret = newlen;
out:
	if (wait_for_pid(pid))
		WARN("do_cgm_get exited with error");
	return ret;
}