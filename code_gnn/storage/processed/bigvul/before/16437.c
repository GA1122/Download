CStarter::transferOutput( void )
{
	UserProc *job;
	bool transient_failure = false;

	if (jic->transferOutput(transient_failure) == false) {

		if( transient_failure ) {
			return false;
		}


		m_reaped_job_list.Rewind();
		while ((job = m_reaped_job_list.Next()) != NULL) {
			ClassAd ad;
			int pid;
			job->PublishUpdateAd(&ad);
			if (ad.LookupInteger(ATTR_JOB_PID, pid)) {
				jic->notifyJobTermination(job);
				break;
			}
		}

		jic->transferOutputMopUp();

			 
		dprintf( D_ALWAYS, "JIC::transferOutput() failed, waiting for job "
				 "lease to expire or for a reconnect attempt\n" );
		return false;
	}

	jic->transferOutputMopUp();

	return cleanupJobs();
}
