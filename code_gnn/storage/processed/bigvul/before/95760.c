static void FS_CheckMPPaks( void )
{
	searchpath_t	*path;
	pack_t		*curpack;
	unsigned int foundPak = 0;

	for( path = fs_searchpaths; path; path = path->next )
	{
		const char* pakBasename = path->pack->pakBasename;

		if(!path->pack)
			continue;
		
		curpack = path->pack;

		if(!Q_stricmpn( curpack->pakGamename, BASEGAME, MAX_OSPATH )
			&& strlen(pakBasename) == 7 && !Q_stricmpn( pakBasename, "mp_pak", 6 )
			&& pakBasename[6] >= '0' && pakBasename[6] <= '0' + NUM_MP_PAKS - 1)
		{
			if( curpack->checksum != mppak_checksums[pakBasename[6]-'0'] )
			{
				if(pakBasename[6] == '0')
				{
					Com_Printf("\n\n"
						"**************************************************\n"
						"WARNING: " BASEGAME "/mp_pak0.pk3 is present but its checksum (%u)\n"
						"is not correct. Please re-copy mp_pak0.pk3 from your\n"
						"legitimate RTCW CDROM.\n"
						"**************************************************\n\n\n",
						curpack->checksum );
				}
				else
				{
					Com_Printf("\n\n"
						"**************************************************\n"
						"WARNING: " BASEGAME "/mp_pak%d.pk3 is present but its checksum (%u)\n"
						"is not correct. Please re-install the point release\n"
						"**************************************************\n\n\n",
						pakBasename[6]-'0', curpack->checksum );
				}
			}

			foundPak |= 1<<(pakBasename[6]-'0');
		}
		else
		{
			int index;
			
			
			for(index = 0; index < ARRAY_LEN(mppak_checksums); index++)
			{
				if(curpack->checksum == mppak_checksums[index])
				{
					Com_Printf("\n\n"
						"**************************************************\n"
						"WARNING: %s is renamed pak file %s%cmp_pak%d.pk3\n"
						"Running in standalone mode won't work\n"
						"Please rename, or remove this file\n"
						"**************************************************\n\n\n",
						curpack->pakFilename, BASEGAME, PATH_SEP, index);


					foundPak |= 0x80000000;
				}
			}

		}
	}

	if(!foundPak && Q_stricmp(com_basegame->string, BASEGAME))
	{
		Cvar_Set("com_standalone", "1");
	}
	else
		Cvar_Set("com_standalone", "0");


	if(!com_standalone->integer && (foundPak & 0x3f) != 0x3f)
	{
		char errorText[MAX_STRING_CHARS] = "";
		char missingPaks[MAX_STRING_CHARS] = "";
		int i = 0;

		if((foundPak & 0x3f) != 0x3f)
		{
			for( i = 0; i < NUM_MP_PAKS; i++ ) {
				if ( !( foundPak & ( 1 << i ) ) ) {
					Q_strcat( missingPaks, sizeof( missingPaks ), va( "mp_pak%d.pk3 ", i ) );
				}
			}

			Q_strcat( errorText, sizeof( errorText ),
				va( "\n\nPoint Release files are missing: %s \n"
				"Please re-install the 1.41 point release.\n\n", missingPaks ) );
		}

		Com_Error(ERR_FATAL, "%s", errorText);
	}
}
