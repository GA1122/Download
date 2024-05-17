qboolean CL_CheckTranslationString( char *original, char *translated ) {
	char format_org[128], format_trans[128];
	int len, i;

	memset( format_org, 0, 128 );
	memset( format_trans, 0, 128 );

	len = strlen( original );

	for ( i = 0; i < len; i++ ) {
		if ( original[i] != '%' ) {
			continue;
		}

		strcat( format_org, va( "%c%c ", '%', original[i + 1] ) );
	}

	len = strlen( translated );
	if ( !len ) {
		return qtrue;
	}

	for ( i = 0; i < len; i++ ) {
		if ( translated[i] != '%' ) {
			continue;
		}

		strcat( format_trans, va( "%c%c ", '%', translated[i + 1] ) );
	}

	len = strlen( format_org );

	if ( len != strlen( format_trans ) ) {
		return qfalse;
	}

	for ( i = 0; i < len; i++ ) {
		if ( format_org[i] != format_trans[i] ) {
			return qfalse;
		}
	}

	return qtrue;
}
