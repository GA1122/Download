CStarter::jobEnvironmentReady( void )
{
#if defined(LINUX)
	GLExecPrivSepHelper* gpsh = glexecPrivSepHelper();
	if (gpsh != NULL) {
		MyString proxy_path;
		if (!jic->jobClassAd()->LookupString(ATTR_X509_USER_PROXY,
		                                     proxy_path))
		{
			EXCEPT("configuration specifies use of glexec, "
			           "but job has no proxy");
		}
		const char* proxy_name = condor_basename(proxy_path.Value());
		gpsh->initialize(proxy_name, WorkingDir.Value());
	}
#endif

	if (m_privsep_helper != NULL) {
		int univ = -1;
		if (!jic->jobClassAd()->LookupInteger(ATTR_JOB_UNIVERSE, univ) ||
		    (univ != CONDOR_UNIVERSE_VM))
		{
			PrivSepError err;
			if( !m_privsep_helper->chown_sandbox_to_user(err) ) {
				jic->notifyStarterError(
					err.holdReason(),
					true,
					err.holdCode(),
					err.holdSubCode());
				EXCEPT("failed to chown sandbox to user");
			}
		}
		else if( univ == CONDOR_UNIVERSE_VM ) {
			m_privsep_helper->set_sandbox_owned_by_user();
		}
	}

	m_job_environment_is_ready = true;

	this->jobWaitUntilExecuteTime( );
	return ( true );
}
