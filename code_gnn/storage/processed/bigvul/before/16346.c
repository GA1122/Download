BaseShadow::requeueJob( const char* reason )
{
	if( ! jobAd ) {
		dprintf( D_ALWAYS, "In requeueJob() w/ NULL JobAd!" );
	}
	dprintf( D_ALWAYS, 
			 "Job %d.%d is being put back in the job queue: %s\n", 
			 getCluster(), getProc(), reason );

	cleanUp();

	int size = strlen( reason ) + strlen( ATTR_REQUEUE_REASON ) + 4;
	char* buf = (char*)malloc( size * sizeof(char) );
	if( ! buf ) {
		EXCEPT( "Out of memory!" );
	}
	sprintf( buf, "%s=\"%s\"", ATTR_REQUEUE_REASON, reason );
	jobAd->Insert( buf );
	free( buf );

	logRequeueEvent( reason );

	if( !updateJobInQueue(U_REQUEUE) ) {
		dprintf( D_ALWAYS, "Failed to update job queue!\n" );
	}

	DC_Exit( JOB_SHOULD_REQUEUE );
}
