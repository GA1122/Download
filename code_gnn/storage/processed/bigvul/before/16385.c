CStarter::Continue( void )
{
	dprintf(D_ALWAYS, "Continuing all jobs.\n");

	UserProc *job;
	m_job_list.Rewind();
	while ((job = m_job_list.Next()) != NULL) {
		job->Continue();
	}
	
	this->suspended = false;

	return ( true );
}
