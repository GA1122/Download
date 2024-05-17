BaseShadow::retryJobCleanupHandler( void )
{
	m_cleanup_retry_tid = -1;
	dprintf(D_ALWAYS, "Retrying job cleanup, calling terminateJob()\n");
	terminateJob();
	return TRUE;
}
