qboolean FS_FilenameCompare( const char *s1, const char *s2 ) {
	int c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;

		if ( Q_islower( c1 ) ) {
			c1 -= ( 'a' - 'A' );
		}
		if ( Q_islower( c2 ) ) {
			c2 -= ( 'a' - 'A' );
		}

		if ( c1 == '\\' || c1 == ':' ) {
			c1 = '/';
		}
		if ( c2 == '\\' || c2 == ':' ) {
			c2 = '/';
		}

		if ( c1 != c2 ) {
			return qtrue;       
		}
	} while ( c1 );

	return qfalse;        
}
