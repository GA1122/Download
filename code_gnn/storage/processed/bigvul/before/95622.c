void Com_WriteConfiguration( void ) {
#if !defined(DEDICATED) && !defined(STANDALONE)
	cvar_t  *fs;
#endif
	if ( !com_fullyInitialized ) {
		return;
	}

	if ( !( cvar_modifiedFlags & CVAR_ARCHIVE ) ) {
		return;
	}
	cvar_modifiedFlags &= ~CVAR_ARCHIVE;

	Com_WriteConfigToFile( Q3CONFIG_CFG );

#if !defined(DEDICATED) && !defined(STANDALONE)
	fs = Cvar_Get( "fs_game", "", CVAR_INIT | CVAR_SYSTEMINFO );
	if(!com_standalone->integer)
	{
		if (UI_usesUniqueCDKey() && fs && fs->string[0] != 0) {
			Com_WriteCDKey( fs->string, &cl_cdkey[16] );
		} else {
			Com_WriteCDKey( BASEGAME, cl_cdkey );
		}
	}
#endif
}
