server_root_strip(const char *path, int n)
{
	const char *p;

	 
	for (; n > 0 && *path != '\0'; n--)
		if ((p = strchr(++path, '/')) == NULL)
			path = strchr(path, '\0');
		else
			path = p;

	return (path);
}