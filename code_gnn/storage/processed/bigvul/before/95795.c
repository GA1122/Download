void FS_InitFilesystem( void ) {
	Com_StartupVariable("fs_basepath");
	Com_StartupVariable("fs_homepath");
	Com_StartupVariable("fs_game");

	if(!FS_FilenameCompare(Cvar_VariableString("fs_game"), com_basegame->string))
		Cvar_Set("fs_game", "");

	FS_Startup(com_basegame->string);

#ifndef STANDALONE
#ifndef UPDATE_SERVER
	FS_CheckPak0( );
#endif
#endif

#ifndef UPDATE_SERVER
	if ( FS_ReadFile( "default.cfg", NULL ) <= 0 ) {
		Com_Error( ERR_FATAL, "Couldn't load default.cfg - I am missing essential files - verify your installation?" );
	}
#endif

	Q_strncpyz( lastValidBase, fs_basepath->string, sizeof( lastValidBase ) );
	Q_strncpyz( lastValidComBaseGame, com_basegame->string, sizeof( lastValidComBaseGame ) );
	Q_strncpyz( lastValidFsBaseGame, fs_basegame->string, sizeof( lastValidFsBaseGame ) );
	Q_strncpyz( lastValidGame, fs_gamedirvar->string, sizeof( lastValidGame ) );
}
