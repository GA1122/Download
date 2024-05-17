static void FS_CheckPak0( void )
{
	searchpath_t	*path;
	pack_t		*curpack;
	qboolean founddemo = qfalse;
	unsigned int foundPak = 0;

	for( path = fs_searchpaths; path; path = path->next )
	{
		const char* pakBasename = path->pack->pakBasename;

		if(!path->pack)
			continue;
		
		curpack = path->pack;

		if(!Q_stricmpn( curpack->pakGamename, "demomain", MAX_OSPATH )
			&& !Q_stricmpn( pakBasename, "pak0", MAX_OSPATH ))
		{
			if(curpack->checksum == DEMO_PAK0_CHECKSUM)
				founddemo = qtrue;
		}

		else if(!Q_stricmpn( curpack->pakGamename, BASEGAME, MAX_OSPATH )
			&& strlen(pakBasename) == 4 && !Q_stricmpn( pakBasename, "pak", 3 )
			&& pakBasename[3] >= '0' && pakBasename[3] <= '0' + NUM_ID_PAKS - 1)
		{
			if( curpack->checksum != pak_checksums[pakBasename[3]-'0'] )
			{
				if(pakBasename[3] == '0')
				{
					Com_Printf("\n\n"
						"**************************************************\n"
						"WARNING: " BASEGAME "/pak0.pk3 is present but its checksum (%u)\n"
						"is not correct. Please re-copy pak0.pk3 from your\n"
						"legitimate RTCW CDROM.\n"
						"**************************************************\n\n\n",
						curpack->checksum );

						Com_Error(ERR_FATAL, NULL);
				}
				 
			}

			foundPak |= 1<<(pakBasename[3]-'0');
		}
		else
		{
			int index;
			
			
			for(index = 0; index < ARRAY_LEN(pak_checksums); index++)
			{
				if(curpack->checksum == pak_checksums[index])
				{
					Com_Printf("\n\n"
						"**************************************************\n"
						"WARNING: %s is renamed pak file %s%cpak%d.pk3\n"
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

	if(!com_standalone->integer)
	{
		if(!(foundPak & 0x01))
		{
			if(founddemo)
			{
				Com_Printf( "\n\n"
					"**************************************************\n"
					"WARNING: It looks like you're using pak0.pk3\n"
					"from the demo. This may work fine, but it is not\n"
					"guaranteed or supported.\n"
					"**************************************************\n\n\n" );
				
				foundPak |= 0x01;
			}
		}
	}


	if(!com_standalone->integer && (foundPak & 0x01) != 0x01)
	{
		char errorText[MAX_STRING_CHARS] = "";

		if((foundPak & 0x01) != 0x01)
		{
			Q_strcat(errorText, sizeof(errorText),
				"\n\n\"pak0.pk3\" is missing. Please copy it\n"
				"from your legitimate RTCW CDROM.\n\n");
		}

		Q_strcat(errorText, sizeof(errorText),
			va("Also check that your iortcw executable is in\n"
				"the correct place and that every file\n"
				"in the \"%s\" directory is present and readable.\n\n", BASEGAME));

		Com_Error(ERR_FATAL, "%s", errorText);
	}

	if(!founddemo)
		FS_CheckMPPaks();
	
}
