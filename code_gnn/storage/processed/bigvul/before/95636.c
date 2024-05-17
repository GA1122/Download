static trans_t* AllocTrans( char *original, char *translated[MAX_LANGUAGES] ) {
	trans_t *t;
	int i;

	t = malloc( sizeof( trans_t ) );
	memset( t, 0, sizeof( trans_t ) );

	if ( original ) {
		strncpy( t->original, original, MAX_TRANS_STRING );
	}

	if ( translated ) {
		for ( i = 0; i < MAX_LANGUAGES; i++ )
			strncpy( t->translated[i], translated[i], MAX_TRANS_STRING );
	}

	return t;
}
