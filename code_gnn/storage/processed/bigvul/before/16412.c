CStarter::allJobsDone( void )
{
	m_all_jobs_done = true;
	bool bRet=false;

	if (m_privsep_helper != NULL) {
		if (jobUniverse != CONDOR_UNIVERSE_VM) {
			PrivSepError err;
			if( !m_privsep_helper->chown_sandbox_to_condor(err) ) {
				jic->notifyStarterError(
					err.holdReason(),
					false,
					err.holdCode(),
					err.holdSubCode());
				EXCEPT("failed to chown sandbox to condor after job completed");
			}
		}
	}

	if (jic->allJobsDone()) {
		bRet=transferOutput();
	}
	
	if (m_deferred_job_update){
		jic->notifyJobExit( -1, JOB_SHOULD_REQUEUE, 0 );
	}
	return bRet;
}
