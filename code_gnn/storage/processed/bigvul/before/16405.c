CStarter::ShutdownGraceful( void )
{
	bool jobRunning = false;
	UserProc *job;

	dprintf(D_ALWAYS, "ShutdownGraceful all jobs.\n");

	if ( this->deferral_tid != -1 ) {
		this->removeDeferredJobs();
	}

	m_job_list.Rewind();
	while ((job = m_job_list.Next()) != NULL) {
		if ( job->ShutdownGraceful() ) {
			m_job_list.DeleteCurrent();
			delete job;
		} else {
			jobRunning = true;
		}
	}
	ShuttingDown = TRUE;
	if (!jobRunning) {
		dprintf(D_FULLDEBUG, 
				"Got ShutdownGraceful when no jobs running.\n");
		this->allJobsDone();
		return 1;
	}	
	return 0;
}
