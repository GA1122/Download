BaseShadow::holdJob( const char* reason, int hold_reason_code, int hold_reason_subcode )
{
	dprintf( D_ALWAYS, "Job %d.%d going into Hold state (code %d,%d): %s\n", 
			 getCluster(), getProc(), hold_reason_code, hold_reason_subcode,reason );

	if( ! jobAd ) {
		dprintf( D_ALWAYS, "In HoldJob() w/ NULL JobAd!" );
		DC_Exit( JOB_SHOULD_HOLD );
	}

	cleanUp();

	jobAd->Assign( ATTR_HOLD_REASON, reason );
	jobAd->Assign( ATTR_HOLD_REASON_CODE, hold_reason_code );
	jobAd->Assign( ATTR_HOLD_REASON_SUBCODE, hold_reason_subcode );

	emailHoldEvent( reason );

	if( !updateJobInQueue(U_HOLD) ) {
		dprintf( D_ALWAYS, "Failed to update job queue!\n" );
	}

}
