static void CL_Cache_SetIndex_f( void ) {
	if ( Cmd_Argc() < 2 ) {
		Com_Error( ERR_DROP, "setindex needs an index\n" );
		return;
	}

	cacheIndex = atoi( Cmd_Argv( 1 ) );
}
