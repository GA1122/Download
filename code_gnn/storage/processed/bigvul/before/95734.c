void CL_TranslateString( const char *string, char *dest_buffer ) {
	int i, count, currentLanguage;
	trans_t *t;
	qboolean newline = qfalse;
	char *buf;

	buf = dest_buffer;
	currentLanguage = cl_language->integer - 1;

	if ( !string ) {
		strcpy( buf, "(null)" );
		return;
	} else if ( currentLanguage == -1 || currentLanguage >= MAX_LANGUAGES || !strlen( string ) )   {
		strcpy( buf, string );
		return;
	}
	if ( string[strlen( string ) - 1] == '\n' ) {
		newline = qtrue;
	}

	for ( i = 0, count = 0; string[i] != '\0'; i++ ) {
		if ( string[i] != '\n' ) {
			buf[count++] = string[i];
		}
	}
	buf[count] = '\0';

	t = LookupTrans( buf, NULL, qfalse );

	if ( t && strlen( t->translated[currentLanguage] ) ) {
		int offset = 0;

		if ( cl_debugTranslation->integer >= 1 ) {
			buf[0] = '^';
			buf[1] = '1';
			buf[2] = '[';
			offset = 3;
		}

		strcpy( buf + offset, t->translated[currentLanguage] );

		if ( cl_debugTranslation->integer >= 1 ) {
			int len2 = strlen( buf );

			buf[len2] = ']';
			buf[len2 + 1] = '^';
			buf[len2 + 2] = '7';
			buf[len2 + 3] = '\0';
		}

		if ( newline ) {
			int len2 = strlen( buf );

			buf[len2] = '\n';
			buf[len2 + 1] = '\0';
		}
	} else {
		int offset = 0;

		if ( cl_debugTranslation->integer >= 1 ) {
			buf[0] = '^';
			buf[1] = '1';
			buf[2] = '[';
			offset = 3;
		}

		strcpy( buf + offset, string );

		if ( cl_debugTranslation->integer >= 1 ) {
			int len2 = strlen( buf );
			qboolean addnewline = qfalse;

			if ( buf[len2 - 1] == '\n' ) {
				len2--;
				addnewline = qtrue;
			}

			buf[len2] = ']';
			buf[len2 + 1] = '^';
			buf[len2 + 2] = '7';
			buf[len2 + 3] = '\0';

			if ( addnewline ) {
				buf[len2 + 3] = '\n';
				buf[len2 + 4] = '\0';
			}
		}
	}
}
