void CL_OpenURL( const char *url ) {
	if ( !url || !strlen( url ) ) {
		Com_Printf( "%s", CL_TranslateStringBuf( "invalid/empty URL\n" ) );
		return;
	}
	Sys_OpenURL( url, qtrue );
}
