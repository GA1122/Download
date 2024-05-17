static int cgroup_read_from_file(const char *fn, char buf[], size_t bufsize)
{
	int ret = lxc_read_from_file(fn, buf, bufsize);
	if (ret < 0) {
		SYSERROR("failed to read %s", fn);
		return ret;
	}
	if (ret == bufsize) {
		if (bufsize > 0) {
			 
			buf[bufsize-1] = '\0';
			return ret;
		}
		 
		ERROR("%s: was not expecting 0 bufsize", __func__);
		return -1;
	}
	buf[ret] = '\0';
	return ret;
}
