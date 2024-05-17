static trans_t* LookupTrans( char *original, char *translated[MAX_LANGUAGES], qboolean isLoading ) {
	trans_t *t, *newt, *prev = NULL;
	long hash;

	hash = generateHashValue( original );

	for ( t = transTable[hash]; t; prev = t, t = t->next ) {
		if ( !Q_stricmp( original, t->original ) ) {
			if ( isLoading ) {
				Com_DPrintf( S_COLOR_YELLOW "WARNING: Duplicate string found: \"%s\"\n", original );
			}
			return t;
		}
	}

	newt = AllocTrans( original, translated );

	if ( prev ) {
		prev->next = newt;
	} else {
		transTable[hash] = newt;
	}

	if ( cl_debugTranslation->integer >= 1 && !isLoading ) {
		Com_Printf( "Missing translation: \'%s\'\n", original );
	}


	return newt;
}
