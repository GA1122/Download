CStarter::Suspend( void ) {
	dprintf(D_ALWAYS, "Suspending all jobs.\n");

	UserProc *job;
	m_job_list.Rewind();
	while ((job = m_job_list.Next()) != NULL) {
		job->Suspend();
	}
	
	this->suspended = true;

	return ( true );
}
