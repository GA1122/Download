CStarter::cleanupJobs( void )
{
	UserProc *job;
	m_reaped_job_list.Rewind();
	while( (job = m_reaped_job_list.Next()) != NULL) {
		if( job->JobExit() ) {
			m_reaped_job_list.DeleteCurrent();
			delete job;
		} else {
			dprintf( D_ALWAYS, "JobExit() failed, waiting for job "
					 "lease to expire or for a reconnect attempt\n" );
			return false;
		}
	}
	jic->allJobsGone();
	return true;
}
