void MSG_WriteBigString( msg_t *sb, const char *s ) {
	if ( !s ) {
		MSG_WriteData (sb, "", 1);
	} else {
		int		l,i;
		char	string[BIG_INFO_STRING];

		l = strlen( s );
		if ( l >= BIG_INFO_STRING ) {
			Com_Printf( "MSG_WriteString: BIG_INFO_STRING" );
			MSG_WriteData (sb, "", 1);
			return;
		}
		Q_strncpyz( string, s, sizeof( string ) );

		for ( i = 0 ; i < l ; i++ ) {
			if ( ((byte *)string)[i] > 127 || string[i] == '%' ) {
				string[i] = '.';
			}
		}

		MSG_WriteData (sb, string, l+1);
	}
}