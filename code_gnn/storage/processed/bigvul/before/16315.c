BaseShadow::BaseShadow() {
	spool = NULL;
	fsDomain = uidDomain = NULL;
	jobAd = NULL;
	remove_requested = false;
	cluster = proc = -1;
	gjid = NULL;
	core_file_name = NULL;
	scheddAddr = NULL;
	job_updater = NULL;
	ASSERT( !myshadow_ptr );	 
	myshadow_ptr = this;
	exception_already_logged = false;
	began_execution = FALSE;
	reconnect_e_factor = 0.0;
	reconnect_ceiling = 300;
	prev_run_bytes_sent = 0.0;
	prev_run_bytes_recvd = 0.0;
	m_num_cleanup_retries = 0;
	m_max_cleanup_retries = 5;
	m_lazy_queue_update = true;
	m_cleanup_retry_tid = -1;
	m_cleanup_retry_delay = 30;
	m_RunAsNobody = false;
}
