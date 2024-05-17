void CL_SaveNewTranslations_f( void ) {
	char fileName[512];

	if ( Cmd_Argc() != 2 ) {
		Com_Printf( "usage: SaveNewTranslations <filename>\n" );
		return;
	}

	strcpy( fileName, va( "translations/%s.cfg", Cmd_Argv( 1 ) ) );

	CL_SaveTransTable( fileName, qtrue );
}
