void CL_InitTranslation( void ) {
	char    **fileList;
	int numFiles, i;
	char fileName[MAX_QPATH];

	memset( transTable, 0, sizeof( trans_t* ) * FILE_HASH_SIZE );
	CL_LoadTransTable( "scripts/translation.cfg" );

	fileList = FS_ListFiles( "translations", ".cfg", &numFiles );

	for ( i = 0; i < numFiles; i++ ) {
		Com_sprintf( fileName, sizeof (fileName), "translations/%s", fileList[i] );
		CL_LoadTransTable( fileName );
	}
}
