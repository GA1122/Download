BaseShadow::retryJobCleanup( void )
{
	m_num_cleanup_retries++;
	if (m_num_cleanup_retries > m_max_cleanup_retries) {
		dprintf(D_ALWAYS,
		        "Maximum number of job cleanup retry attempts "
		        "(SHADOW_MAX_JOB_CLEANUP_RETRIES=%d) reached"
		        "; Forcing job requeue!\n",
		        m_max_cleanup_retries);
		DC_Exit(JOB_SHOULD_REQUEUE);
	}
	ASSERT(m_cleanup_retry_tid == -1);
	m_cleanup_retry_tid = daemonCore->Register_Timer(m_cleanup_retry_delay, 0,
					(TimerHandlercpp)&BaseShadow::retryJobCleanupHandler,
					"retry job cleanup", this);
	dprintf(D_FULLDEBUG, "Will retry job cleanup in "
	        "SHADOW_JOB_CLEANUP_RETRY_DELAY=%d seconds\n",
	        m_cleanup_retry_delay);
}
