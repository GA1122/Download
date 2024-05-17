static void FS_CheckPak0( void )
{
	searchpath_t	*path;
	pack_t		*curpack;
	qboolean founddemo = qfalse;
	unsigned int foundPak = 0, foundTA = 0;

	for( path = fs_searchpaths; path; path = path->next )
	{
		const char* pakBasename = path->pack->pakBasename;

		if(!path->pack)
			continue;

		curpack = path->pack;

		if(!Q_stricmpn( curpack->pakGamename, "demoq3", MAX_OSPATH )
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
							"legitimate Q3 CDROM.\n"
							"**************************************************\n\n\n",
							curpack->checksum );
				}
				else
				{
					Com_Printf("\n\n"
							"**************************************************\n"
							"WARNING: " BASEGAME "/pak%d.pk3 is present but its checksum (%u)\n"
							"is not correct. Please re-install the point release\n"
							"**************************************************\n\n\n",
							pakBasename[3]-'0', curpack->checksum );
				}
			}

			foundPak |= 1<<(pakBasename[3]-'0');
		}
		else if(!Q_stricmpn(curpack->pakGamename, BASETA, MAX_OSPATH)
				&& strlen(pakBasename) == 4 && !Q_stricmpn(pakBasename, "pak", 3)
				&& pakBasename[3] >= '0' && pakBasename[3] <= '0' + NUM_TA_PAKS - 1)

		{
			if(curpack->checksum != missionpak_checksums[pakBasename[3]-'0'])
			{
				Com_Printf("\n\n"
						"**************************************************\n"
						"WARNING: " BASETA "/pak%d.pk3 is present but its checksum (%u)\n"
						"is not correct. Please re-install Team Arena\n"
						"**************************************************\n\n\n",
						pakBasename[3]-'0', curpack->checksum );				
			}

			foundTA |= 1 << (pakBasename[3]-'0');
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

			for(index = 0; index < ARRAY_LEN(missionpak_checksums); index++)
			{
				if(curpack->checksum == missionpak_checksums[index])
				{
					Com_Printf("\n\n"
							"**************************************************\n"
							"WARNING: %s is renamed pak file %s%cpak%d.pk3\n"
							"Running in standalone mode won't work\n"
							"Please rename, or remove this file\n"
							"**************************************************\n\n\n",
							curpack->pakFilename, BASETA, PATH_SEP, index);

					foundTA |= 0x80000000;
				}
			}
		}
	}

	if(!foundPak && !foundTA && Q_stricmp(com_basegame->string, BASEGAME))
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


	if(!com_standalone->integer && (foundPak & 0x1ff) != 0x1ff)
	{
		char errorText[MAX_STRING_CHARS] = "";

		if((foundPak & 0x01) != 0x01)
		{
			Q_strcat(errorText, sizeof(errorText),
					"\"pak0.pk3\" is missing. Please copy it "
					"from your legitimate Q3 CDROM. ");
		}

		if((foundPak & 0x1fe) != 0x1fe)
		{
			Q_strcat(errorText, sizeof(errorText),
					"Point Release files are missing. Please "
					"re-install the 1.32 point release. ");
		}

		Q_strcat(errorText, sizeof(errorText),
				va("Also check that your ioq3 executable is in "
					"the correct place and that every file "
					"in the \"%s\" directory is present and readable", BASEGAME));

		Com_Error(ERR_FATAL, "%s", errorText);
	}

	if(!com_standalone->integer && foundTA && (foundTA & 0x0f) != 0x0f)
	{
		char errorText[MAX_STRING_CHARS] = "";

		if((foundTA & 0x01) != 0x01)
		{
			Com_sprintf(errorText, sizeof(errorText),
					"\"" BASETA "%cpak0.pk3\" is missing. Please copy it "
					"from your legitimate Quake 3 Team Arena CDROM. ", PATH_SEP);
		}

		if((foundTA & 0x0e) != 0x0e)
		{
			Q_strcat(errorText, sizeof(errorText),
					"Team Arena Point Release files are missing. Please "
					"re-install the latest Team Arena point release.");
		}

		Com_Error(ERR_FATAL, "%s", errorText);
	}
}
