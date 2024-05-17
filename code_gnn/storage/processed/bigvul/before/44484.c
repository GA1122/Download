static bool is_valid_cgroup(const char *name)
{
	const char *p;
	for (p = name; *p; p++) {
		 
		if (*p <= 32 || *p >= 127 || *p == '/')
			return false;
	}
	return strcmp(name, ".") != 0 && strcmp(name, "..") != 0;
}
