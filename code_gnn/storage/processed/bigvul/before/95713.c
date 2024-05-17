void CL_SaveTransTable( const char *fileName, qboolean newOnly ) {
	int bucketlen, bucketnum, maxbucketlen, avebucketlen;
	int untransnum, transnum;
	const char *buf;
	fileHandle_t f;
	trans_t *t;
	int i, j, len;

	if ( cl.corruptedTranslationFile ) {
		Com_Printf( S_COLOR_YELLOW "WARNING: Cannot save corrupted translation file. Please reload first." );
		return;
	}

	FS_FOpenFileByMode( fileName, &f, FS_WRITE );

	bucketnum = 0;
	maxbucketlen = 0;
	avebucketlen = 0;
	transnum = 0;
	untransnum = 0;

	if ( strlen( cl.translationVersion ) ) {
		buf = va( "#version\t\t\"%s\"\n", cl.translationVersion );
	} else {
		buf = va( "#version\t\t\"1.0 01/01/01\"\n" );
	}

	len = strlen( buf );
	FS_Write( buf, len, f );

	for ( j = 0; j < 2; j++ ) {

		for ( i = 0; i < FILE_HASH_SIZE; i++ ) {
			t = transTable[i];

			if ( !t || ( newOnly && t->fromFile ) ) {
				continue;
			}

			bucketlen = 0;

			for ( ; t; t = t->next ) {
				bucketlen++;

				if ( strlen( t->translated[0] ) ) {
					if ( j ) {
						continue;
					}
					transnum++;
				} else {
					if ( !j ) {
						continue;
					}
					untransnum++;
				}

				buf = va( "{\n\tenglish\t\t\"%s\"\n", t->original );
				len = strlen( buf );
				FS_Write( buf, len, f );

				buf = va( "\tfrench\t\t\"%s\"\n", t->translated[LANGUAGE_FRENCH] );
				len = strlen( buf );
				FS_Write( buf, len, f );

				buf = va( "\tgerman\t\t\"%s\"\n", t->translated[LANGUAGE_GERMAN] );
				len = strlen( buf );
				FS_Write( buf, len, f );

				buf = va( "\titalian\t\t\"%s\"\n", t->translated[LANGUAGE_ITALIAN] );
				len = strlen( buf );
				FS_Write( buf, len, f );

				buf = va( "\tspanish\t\t\"%s\"\n", t->translated[LANGUAGE_SPANISH] );
				len = strlen( buf );
				FS_Write( buf, len, f );

				buf = va( "}\n" );
				len = strlen( buf );
				FS_Write( buf, len, f );
			}

			if ( bucketlen > maxbucketlen ) {
				maxbucketlen = bucketlen;
			}

			if ( bucketlen ) {
				bucketnum++;
				avebucketlen += bucketlen;
			}
		}
	}

	Com_Printf( "Saved translation table.\nTotal = %i, Translated = %i, Untranslated = %i, aveblen = %2.2f, maxblen = %i\n",
				transnum + untransnum, transnum, untransnum, (float)avebucketlen / bucketnum, maxbucketlen );

	FS_FCloseFile( f );
}
