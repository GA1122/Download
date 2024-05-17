uucp_lockname(const char *dir, const char *file)
{
	char *p, *cp;
	struct stat sb;

	if ( ! dir || *dir == '\0' || stat(dir, &sb) != 0 )
		return -1;

	 
	p = strchr(file + 1, '/');
	p = p ? p + 1 : (char *)file;
	 
	p = cp = strdup(p);
	do { if ( *p == '/' ) *p = '_'; } while(*p++);
	 
	snprintf(lockname, sizeof(lockname), "%s/LCK..%s", dir, cp);
	 
	free(cp);

	return 0;
}
