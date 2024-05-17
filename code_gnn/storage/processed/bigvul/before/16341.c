BaseShadow::mockTerminateJob( MyString exit_reason, 
		bool exited_by_signal, int exit_code, int exit_signal, 
		bool core_dumped )
{
	if (exit_reason == "") {
		exit_reason = "Exited normally";
	}
	
	dprintf( D_ALWAYS, "Mock terminating job %d.%d: "
			"exited_by_signal=%s, exit_code=%d OR exit_signal=%d, "
			"core_dumped=%s, exit_reason=\"%s\"\n", 
			 getCluster(),
			 getProc(), 
			 exited_by_signal ? "TRUE" : "FALSE",
			 exit_code,
			 exit_signal,
			 core_dumped ? "TRUE" : "FALSE",
			 exit_reason.Value());

	if( ! jobAd ) {
		dprintf(D_ALWAYS, "BaseShadow::mockTerminateJob(): NULL JobAd! "
			"Holding Job!");
		DC_Exit( JOB_SHOULD_HOLD );
	}

	jobAd->Assign( ATTR_JOB_CORE_DUMPED, core_dumped );
	jobAd->Assign( ATTR_ON_EXIT_BY_SIGNAL, exited_by_signal );

	if (exited_by_signal) {
		jobAd->Assign( ATTR_ON_EXIT_SIGNAL, exit_signal );
	} else {
		jobAd->Assign( ATTR_ON_EXIT_CODE, exit_code );
	}

	jobAd->Assign( ATTR_EXIT_REASON, exit_reason );

	if( !updateJobInQueue(U_TERMINATE) ) {
		dprintf( D_ALWAYS, "Failed to update job queue!\n" );
	}
}
