BaseShadow::evictJob( int reason )
{
	MyString from_where;
	MyString machine;
	if( getMachineName(machine) ) {
		from_where.sprintf(" from %s",machine.Value());
	}
	dprintf( D_ALWAYS, "Job %d.%d is being evicted%s\n",
			 getCluster(), getProc(), from_where.Value() );

	if( ! jobAd ) {
		dprintf( D_ALWAYS, "In evictJob() w/ NULL JobAd!" );
		DC_Exit( reason );
	}

	cleanUp();

	logEvictEvent( reason );

	char buf[64];
	sprintf( buf, "%s = %d", ATTR_LAST_VACATE_TIME, (int)time(0) ); 
	jobAd->Insert( buf );

	if( !updateJobInQueue(U_EVICT) ) {
		dprintf( D_ALWAYS, "Failed to update job queue!\n" );
	}

	DC_Exit( reason );
}
