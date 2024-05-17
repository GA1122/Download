void BaseShadow::removeJobPre( const char* reason )
{
	if( ! jobAd ) {
		dprintf( D_ALWAYS, "In removeJob() w/ NULL JobAd!" );
	}
	dprintf( D_ALWAYS, "Job %d.%d is being removed: %s\n", 
			 getCluster(), getProc(), reason );

	cleanUp();

	int size = strlen( reason ) + strlen( ATTR_REMOVE_REASON ) + 4;
	char* buf = (char*)malloc( size * sizeof(char) );
	if( ! buf ) {
		EXCEPT( "Out of memory!" );
	}
	sprintf( buf, "%s=\"%s\"", ATTR_REMOVE_REASON, reason );
	jobAd->Insert( buf );
	free( buf );

	emailRemoveEvent( reason );

	if( !updateJobInQueue(U_REMOVE) ) {
		dprintf( D_ALWAYS, "Failed to update job queue!\n" );
	}
}
