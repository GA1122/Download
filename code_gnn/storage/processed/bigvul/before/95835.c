static void FS_Startup( const char *gameName ) {
	const char *homePath;

	Com_Printf( "----- FS_Startup -----\n" );

	fs_packFiles = 0;

	fs_debug = Cvar_Get( "fs_debug", "0", 0 );
	fs_basepath = Cvar_Get ("fs_basepath", Sys_DefaultInstallPath(), CVAR_INIT|CVAR_PROTECTED );
	fs_basegame = Cvar_Get( "fs_basegame", "", CVAR_INIT );
	homePath = Sys_DefaultHomePath();
	if (!homePath || !homePath[0]) {
		homePath = fs_basepath->string;
	}
	fs_homepath = Cvar_Get ("fs_homepath", homePath, CVAR_INIT|CVAR_PROTECTED );
	fs_gamedirvar = Cvar_Get( "fs_game", "", CVAR_INIT | CVAR_SYSTEMINFO );

#ifndef STANDALONE
	fs_steampath = Cvar_Get ("fs_steampath", Sys_SteamPath(), CVAR_INIT|CVAR_PROTECTED );
	if (fs_steampath->string[0]) {
		FS_AddGameDirectory( fs_steampath->string, gameName, qtrue );
	}
#endif

	if ( fs_basepath->string[0] ) {
		FS_AddGameDirectory( fs_basepath->string, gameName, qtrue );
	}

#ifdef __APPLE__
	fs_apppath = Cvar_Get ("fs_apppath", Sys_DefaultAppPath(), CVAR_INIT|CVAR_PROTECTED );
	if (fs_apppath->string[0])
		FS_AddGameDirectory(fs_apppath->string, gameName, qtrue);
#endif
	
	if (fs_homepath->string[0] && Q_stricmp(fs_homepath->string,fs_basepath->string)) {
		FS_CreatePath ( fs_homepath->string );
		FS_AddGameDirectory( fs_homepath->string, gameName, qtrue );
	}

	if ( fs_basegame->string[0] && Q_stricmp( fs_basegame->string, gameName ) ) {
#ifndef STANDALONE
		if ( fs_steampath->string[0] ) {
			FS_AddGameDirectory( fs_steampath->string, fs_basegame->string, qtrue );
		}
#endif

		if ( fs_basepath->string[0] ) {
			FS_AddGameDirectory( fs_basepath->string, fs_basegame->string, qtrue );
		}

		if ( fs_homepath->string[0] && Q_stricmp( fs_homepath->string,fs_basepath->string ) ) {
			FS_AddGameDirectory( fs_homepath->string, fs_basegame->string, qtrue );
		}
	}

	if ( fs_gamedirvar->string[0] && Q_stricmp( fs_gamedirvar->string, gameName ) ) {
#ifndef STANDALONE
		if ( fs_steampath->string[0] ) {
			FS_AddGameDirectory( fs_steampath->string, fs_gamedirvar->string, qtrue );
		}
#endif

		if ( fs_basepath->string[0] ) {
			FS_AddGameDirectory( fs_basepath->string, fs_gamedirvar->string, qtrue );
		}

		if ( fs_homepath->string[0] && Q_stricmp( fs_homepath->string,fs_basepath->string ) ) {
			FS_AddGameDirectory( fs_homepath->string, fs_gamedirvar->string, qtrue );
		}
	}

#ifndef STANDALONE
	if(!com_standalone->integer)
	{
		cvar_t	*fs;

		Com_ReadCDKey(BASEGAME);
		fs = Cvar_Get ("fs_game", "", CVAR_INIT|CVAR_SYSTEMINFO );
		if (fs && fs->string[0] != 0) {
			Com_AppendCDKey( fs->string );
		}
	}
#endif

	Cmd_AddCommand( "path", FS_Path_f );
	Cmd_AddCommand( "dir", FS_Dir_f );
	Cmd_AddCommand( "fdir", FS_NewDir_f );
	Cmd_AddCommand( "touchFile", FS_TouchFile_f );
	Cmd_AddCommand ("which", FS_Which_f );

	FS_ReorderPurePaks();

	FS_Path_f();

	fs_gamedirvar->modified = qfalse;  

	Com_Printf( "----------------------\n" );

#ifdef FS_MISSING
	if ( missingFiles == NULL ) {
		missingFiles = Sys_FOpen( "\\missing.txt", "ab" );
	}
#endif
	Com_Printf( "%d files in pk3 files\n", fs_packFiles );
}
