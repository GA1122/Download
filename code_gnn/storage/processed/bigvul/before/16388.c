CStarter::Hold( void )
{	
	bool jobRunning = false;
	UserProc *job;

	dprintf( D_ALWAYS, "Hold all jobs\n" );

	if ( this->deferral_tid != -1 ) {
		this->removeDeferredJobs();
	}

	m_job_list.Rewind();
	while( (job = m_job_list.Next()) != NULL ) {
		if( job->Hold() ) {
			m_job_list.DeleteCurrent();
			delete job;
		} else {
			jobRunning = true;
		}
	}
	ShuttingDown = TRUE;
	return ( !jobRunning );
}
