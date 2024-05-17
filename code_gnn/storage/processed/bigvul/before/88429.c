is_url(const char *source)
{
	char *colonp;

	 
	colonp = strchr(source, ':');
	if (colonp == NULL)
	{
		 
		return (0);
	}

	 
	if (strncmp(colonp + 1, "//", 2) != 0)
	{
		 
		return (0);
	}

	 
	return (1);
}
