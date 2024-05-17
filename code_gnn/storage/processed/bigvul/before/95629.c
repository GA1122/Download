static sfxHandle_t S_AL_BufferFind(const char *filename)
{
	sfxHandle_t sfx = -1;
	int i;

	if ( !filename ) {
		filename = "*default*";
	}

	if ( !filename[0] ) {
		filename = "*default*";
	}

	if ( strlen( filename ) >= MAX_QPATH ) {
		Com_Printf( S_COLOR_YELLOW "WARNING: Sound name is too long: %s\n", filename );
		return 0;
	}

	for(i = 0; i < numSfx; i++)
	{
		if(!Q_stricmp(knownSfx[i].filename, filename))
		{
			sfx = i;
			break;
		}
	}

	if(sfx == -1)
	{
		alSfx_t *ptr;

		sfx = S_AL_BufferFindFree();

		ptr = &knownSfx[sfx];
		memset(ptr, 0, sizeof(*ptr));
		ptr->masterLoopSrc = -1;
		strcpy(ptr->filename, filename);
	}

	return sfx;
}