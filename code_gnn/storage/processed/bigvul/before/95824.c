void FS_Restart( int checksumFeed ) {
	const char *lastGameDir;

	FS_Shutdown( qfalse );

	fs_checksumFeed = checksumFeed;

	FS_ClearPakReferences( 0 );

	FS_Startup(com_basegame->string);

#ifndef STANDALONE
	FS_CheckPak0( );
#endif

	if ( FS_ReadFile( "default.cfg", NULL ) <= 0 ) {
		if ( lastValidBase[0] ) {
			FS_PureServerSetLoadedPaks( "", "" );
			Cvar_Set( "fs_basepath", lastValidBase );
			Cvar_Set( "com_basegame", lastValidComBaseGame );
			Cvar_Set( "fs_basegame", lastValidFsBaseGame );
			Cvar_Set( "fs_game", lastValidGame );
			lastValidBase[0] = '\0';
			lastValidComBaseGame[0] = '\0';
			lastValidFsBaseGame[0] = '\0';
			lastValidGame[0] = '\0';
			FS_Restart( checksumFeed );
			Com_Error( ERR_DROP, "Invalid game folder" );
			return;
		}
		Com_Error( ERR_FATAL, "Couldn't load default.cfg" );
	}

	lastGameDir = ( lastValidGame[0] ) ? lastValidGame : lastValidComBaseGame;

	if ( Q_stricmp( FS_GetCurrentGameDir(), lastGameDir ) ) {
		Sys_RemovePIDFile( lastGameDir );
		Sys_InitPIDFile( FS_GetCurrentGameDir() );

		if ( !Com_SafeMode() ) {
			Cbuf_AddText("exec " Q3CONFIG_CFG "\n");
		}
	}

	Q_strncpyz( lastValidBase, fs_basepath->string, sizeof( lastValidBase ) );
	Q_strncpyz( lastValidComBaseGame, com_basegame->string, sizeof( lastValidComBaseGame ) );
	Q_strncpyz( lastValidFsBaseGame, fs_basegame->string, sizeof( lastValidFsBaseGame ) );
	Q_strncpyz( lastValidGame, fs_gamedirvar->string, sizeof( lastValidGame ) );

}
