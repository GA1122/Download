qboolean FS_Which(const char *filename, void *searchPath)
{
	searchpath_t *search = searchPath;

	if(FS_FOpenFileReadDir(filename, search, NULL, qfalse, qfalse) > 0)
	{
		if(search->pack)
		{
			Com_Printf("File \"%s\" found in \"%s\"\n", filename, search->pack->pakFilename);
			return qtrue;
		}
		else if(search->dir)
		{
			Com_Printf( "File \"%s\" found at \"%s\"\n", filename, search->dir->fullpath);
			return qtrue;
		}
	}

	return qfalse;
}
