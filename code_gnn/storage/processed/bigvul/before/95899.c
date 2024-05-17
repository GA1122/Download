static void FS_CheckSPPaks( void )
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
				&& strlen(pakBasename) == 7 && !Q_stricmpn( pakBasename, "sp_pak", 6 )
				&& pakBasename[6] >= '1' && pakBasename[6] <= '1' + NUM_SP_PAKS - 1)
		{
			if( curpack->checksum != en_sppak_checksums[pakBasename[6]-'1'] &&
				curpack->checksum != fr_sppak_checksums[pakBasename[6]-'1'] &&
				curpack->checksum != gm_sppak_checksums[pakBasename[6]-'1'] &&
				curpack->checksum != it_sppak_checksums[pakBasename[6]-'1'] &&
				curpack->checksum != sp_sppak_checksums[pakBasename[6]-'1'] )
			{
				if(pakBasename[6] == '1')
				{
					Com_Printf("\n\n"
						"**************************************************\n"
						"WARNING: " BASEGAME "/sp_pak1.pk3 is present but its checksum (%u)\n"
						"is not correct. Please re-copy sp_pak1.pk3 from your\n"
						"legitimate RTCW CDROM.\n"
						"**************************************************\n\n\n",
						curpack->checksum );
				}
				else
				{
					Com_Printf("\n\n"
						"**************************************************\n"
						"WARNING: " BASEGAME "/sp_pak%d.pk3 is present but its checksum (%u)\n"
						"is not correct. Please re-install the point release\n"
						"**************************************************\n\n\n",
						pakBasename[6]-'0', curpack->checksum );
				}
			}

			foundPak |= 1<<(pakBasename[6]-'1');
		}
		else
		{
			int index;


			for(index = 0; index < ARRAY_LEN( en_sppak_checksums ); index++)
			{
				if( curpack->checksum == en_sppak_checksums[index] ||
					curpack->checksum == fr_sppak_checksums[index] ||
					curpack->checksum == gm_sppak_checksums[index] ||
					curpack->checksum == it_sppak_checksums[index] ||
					curpack->checksum == sp_sppak_checksums[index] )
				{
					Com_Printf("\n\n"
						"**************************************************\n"
						"WARNING: %s is renamed pak file %s%csp_pak%d.pk3\n"
						"Running in standalone mode won't work\n"
						"Please rename, or remove this file\n"
						"**************************************************\n\n\n",
						curpack->pakFilename, BASEGAME, PATH_SEP, index + 1 );


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


	if(!com_standalone->integer && (foundPak & 0xf) != 0xf)
	{
		char errorText[MAX_STRING_CHARS] = "";
		char missingPaks[MAX_STRING_CHARS] = "";
		int i = 0;

		if((foundPak & 0xf) != 0xf)
		{
			for( i = 0; i < NUM_SP_PAKS; i++ ) {
				if ( !( foundPak & ( 1 << i ) ) ) {
					Q_strcat( missingPaks, sizeof( missingPaks ), va( "sp_pak%d.pk3 ", i + 1 ) );
				}
			}

			Q_strcat( errorText, sizeof( errorText ),
				va( "\n\nPoint Release files are missing: %s \n"
				"Please re-install the 1.41 point release.\n\n", missingPaks ) );
		}

		Com_Error(ERR_FATAL, "%s", errorText);
	}
}
