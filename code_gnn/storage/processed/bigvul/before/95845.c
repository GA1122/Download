static char** Sys_ConcatenateFileLists( char **list0, char **list1 )
{
	int totalLength = 0;
	char** cat = NULL, **dst, **src;

	totalLength += Sys_CountFileList(list0);
	totalLength += Sys_CountFileList(list1);

	 
	dst = cat = Z_Malloc( ( totalLength + 1 ) * sizeof( char* ) );

	 
	if (list0)
	{
		for (src = list0; *src; src++, dst++)
			*dst = *src;
	}
	if (list1)
	{
		for (src = list1; *src; src++, dst++)
			*dst = *src;
	}

	*dst = NULL;

	if (list0) Z_Free( list0 );
	if (list1) Z_Free( list1 );

	return cat;
}
