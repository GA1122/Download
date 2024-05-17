BaseShadow::logTerminateEvent( int exitReason, update_style_t kind )
{
	struct rusage run_remote_rusage;
	JobTerminatedEvent event;
	MyString corefile;

	memset( &run_remote_rusage, 0, sizeof(struct rusage) );

	switch( exitReason ) {
	case JOB_EXITED:
	case JOB_COREDUMPED:
		break;
	default:
		dprintf( D_ALWAYS, 
				 "UserLog logTerminateEvent with unknown reason (%d), aborting\n",
				 exitReason ); 
		return;
	}

	if (kind == US_TERMINATE_PENDING) {

		float float_value;
		int exited_by_signal = FALSE;
		int exit_signal = 0;
		int exit_code = 0;

		getJobAdExitedBySignal(jobAd, exited_by_signal);
		if (exited_by_signal == TRUE) {
			getJobAdExitSignal(jobAd, exit_signal);
			event.normal = false;
			event.signalNumber = exit_signal;
		} else {
			getJobAdExitCode(jobAd, exit_code);
			event.normal = true;
			event.returnValue = exit_code;
		}

		 
		if( jobAd->LookupFloat(ATTR_JOB_REMOTE_SYS_CPU, float_value) ) {
			run_remote_rusage.ru_stime.tv_sec = (int) float_value;
		}

		if( jobAd->LookupFloat(ATTR_JOB_REMOTE_USER_CPU, float_value) ) {
			run_remote_rusage.ru_utime.tv_sec = (int) float_value;
		}

		event.run_remote_rusage = run_remote_rusage;
		event.total_remote_rusage = run_remote_rusage;
	
		 
		jobAd->LookupFloat(ATTR_BYTES_SENT, event.recvd_bytes);
		jobAd->LookupFloat(ATTR_BYTES_RECVD, event.sent_bytes);

		event.total_recvd_bytes = event.recvd_bytes;
		event.total_sent_bytes = event.sent_bytes;
	
		if( exited_by_signal == TRUE ) {
			jobAd->LookupString(ATTR_JOB_CORE_FILENAME, corefile);
			event.setCoreFile( corefile.Value() );
		}

		if (!uLog.writeEvent (&event,jobAd)) {
			dprintf (D_ALWAYS,"Unable to log "
				 	"ULOG_JOB_TERMINATED event\n");
			EXCEPT("UserLog Unable to log ULOG_JOB_TERMINATED event");
		}

		return;
	}


	run_remote_rusage = getRUsage();
	
	if( exitedBySignal() ) {
		event.normal = false;
		event.signalNumber = exitSignal();
	} else {
		event.normal = true;
		event.returnValue = exitCode();
	}

	event.run_remote_rusage = run_remote_rusage;
	event.total_remote_rusage = run_remote_rusage;
	
		 
	event.recvd_bytes = bytesSent();
	event.sent_bytes = bytesReceived();

	event.total_recvd_bytes = prev_run_bytes_recvd + bytesSent();
	event.total_sent_bytes = prev_run_bytes_sent + bytesReceived();
	
	if( exitReason == JOB_COREDUMPED ) {
		event.setCoreFile( core_file_name );
	}
	
	if (!uLog.writeEvent (&event,jobAd)) {
		dprintf (D_ALWAYS,"Unable to log "
				 "ULOG_JOB_TERMINATED event\n");
		EXCEPT("UserLog Unable to log ULOG_JOB_TERMINATED event");
	}
}
