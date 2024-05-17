static unsigned int Sys_CountFileList( char **list ) {
	int i = 0;

	if (list)
	{
		while (*list)
		{
			list++;
			i++;
		}
	}
	return i;
}
