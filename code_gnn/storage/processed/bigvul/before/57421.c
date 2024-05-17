static int logon_vet_description(const char *desc)
{
	char *p;

	 
	p = strchr(desc, ':');
	if (!p)
		return -EINVAL;

	 
	if (p == desc)
		return -EINVAL;

	return 0;
}
