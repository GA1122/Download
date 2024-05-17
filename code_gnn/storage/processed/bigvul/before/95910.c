qboolean FS_FileCompare( const char *s1, const char *s2 ) {
	FILE    *f1, *f2;
	int len1, len2, pos;
	byte    *b1, *b2, *p1, *p2;

	f1 = fopen( s1, "rb" );
	if ( !f1 ) {
		Com_Error( ERR_FATAL, "FS_FileCompare: %s does not exist\n", s1 );
	}

	f2 = fopen( s2, "rb" );
	if ( !f2 ) {   
		fclose( f1 );
		return qfalse;
	}

	pos = ftell( f1 );
	fseek( f1, 0, SEEK_END );
	len1 = ftell( f1 );
	fseek( f1, pos, SEEK_SET );

	pos = ftell( f2 );
	fseek( f2, 0, SEEK_END );
	len2 = ftell( f2 );
	fseek( f2, pos, SEEK_SET );

	if ( len1 != len2 ) {
		fclose( f1 );
		fclose( f2 );
		return qfalse;
	}

	b1 = malloc( len1 );
	if ( fread( b1, 1, len1, f1 ) != len1 ) {
		Com_Error( ERR_FATAL, "Short read in FS_FileCompare()\n" );
	}
	fclose( f1 );

	b2 = malloc( len2 );
	if ( fread( b2, 1, len2, f2 ) != len2 ) {
		Com_Error( ERR_FATAL, "Short read in FS_FileCompare()\n" );
	}
	fclose( f2 );

	p1 = b1;
	p2 = b2;
	for ( pos = 0; pos < len1; pos++, p1++, p2++ )
	{
		if ( *p1 != *p2 ) {
			free( b1 );
			free( b2 );
			return qfalse;
		}
	}

	free( b1 );
	free( b2 );
	return qtrue;
}
