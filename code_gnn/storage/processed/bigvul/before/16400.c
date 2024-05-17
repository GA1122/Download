CStarter::Remove( ) {
	bool jobRunning = false;
	UserProc *job;

	dprintf( D_ALWAYS, "Remove all jobs\n" );

	if ( this->deferral_tid != -1 ) {
		this->removeDeferredJobs();
	}

	m_job_list.Rewind();
	while( (job = m_job_list.Next()) != NULL ) {
		if( job->Remove() ) {
			m_job_list.DeleteCurrent();
			delete job;
		} else {
			jobRunning = true;
		}
	}
	ShuttingDown = TRUE;

	return ( !jobRunning );
}
