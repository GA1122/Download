get_linux_hostspec(const char *solaris_hostspec, char **plinux_hostspec)
{
	 
	if (solaris_hostspec[0] == '@') {
		 
		*plinux_hostspec = strdup(solaris_hostspec + 1);
	} else {
		*plinux_hostspec = strdup(solaris_hostspec);
	}

	if (*plinux_hostspec == NULL) {
		return (SA_NO_MEMORY);
	}

	return (SA_OK);
}
