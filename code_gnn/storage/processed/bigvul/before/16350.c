BaseShadow::shutDown( int reason ) 
{
	if ( !getJobAd() ) {
		DC_Exit( reason );
	}
	
	if ( reason == JOB_EXCEPTION ) {
		return;
	}

	if( reason == JOB_EXITED || reason == JOB_COREDUMPED ) {
		shadow_user_policy.checkAtExit();
	}
	else {
		evictJob( reason );
	}
}
