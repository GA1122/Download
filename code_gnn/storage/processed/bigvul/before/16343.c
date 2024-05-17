BaseShadow::reconnectFailed( const char* reason )
{
	dprintf( D_ALWAYS, "Reconnect FAILED: %s\n", reason );
	
	logReconnectFailedEvent( reason );

	DC_Exit( JOB_SHOULD_REQUEUE );
}
