CStarter::ShutdownFast( void )
{
	bool jobRunning = false;
	UserProc *job;

	dprintf(D_ALWAYS, "ShutdownFast all jobs.\n");
	
	if ( this->deferral_tid != -1 ) {
		this->removeDeferredJobs();
	}

	m_job_list.Rewind();
	while ((job = m_job_list.Next()) != NULL) {
		if ( job->ShutdownFast() ) {
			m_job_list.DeleteCurrent();
			delete job;
		} else {
			jobRunning = true;
		}
	}
	ShuttingDown = TRUE;
	if (!jobRunning) {
		dprintf(D_FULLDEBUG, 
				"Got ShutdownFast when no jobs running.\n");
		return ( this->allJobsDone() );
	}	
	return ( false );
}
