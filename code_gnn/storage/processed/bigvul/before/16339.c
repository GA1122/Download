BaseShadow::logRequeueEvent( const char* reason )
{
	struct rusage run_remote_rusage;
	memset( &run_remote_rusage, 0, sizeof(struct rusage) );

	run_remote_rusage = getRUsage();

	int exit_reason = getExitReason();

	JobEvictedEvent event;

	event.terminate_and_requeued = true;

	if( exitedBySignal() ) {
		event.normal = false;
		event.signal_number = exitSignal();
	} else {
		event.normal = true;
		event.return_value = exitCode();
	}
			
	if( exit_reason == JOB_COREDUMPED ) {
		event.setCoreFile( core_file_name );
	}

	if( reason ) {
		event.setReason( reason );
	}

	event.run_remote_rusage = run_remote_rusage;

		 
	event.recvd_bytes = bytesSent();
	event.sent_bytes = bytesReceived();
	
	if (!uLog.writeEvent (&event,jobAd)) {
		dprintf( D_ALWAYS, "Unable to log ULOG_JOB_EVICTED "
				 "(and requeued) event\n" );
	}
}
