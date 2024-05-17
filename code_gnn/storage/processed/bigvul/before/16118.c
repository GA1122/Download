GahpServer::GahpServer(const char *id, const char *path, const ArgList *args)
{
	m_gahp_pid = -1;
	m_gahp_startup_failed = false;
	m_gahp_readfd = -1;
	m_gahp_writefd = -1;
	m_gahp_errorfd = -1;
	m_gahp_error_buffer = "";
	m_reference_count = 0;
	m_commands_supported = NULL;
	m_pollInterval = 5;
	poll_tid = -1;
	max_pending_requests = param_integer( "GRIDMANAGER_MAX_PENDING_REQUESTS", 50 );
	num_pending_requests = 0;
	poll_pending = false;
	use_prefix = false;
	requestTable = NULL;
	current_proxy = NULL;
	skip_next_r = false;
	m_deleteMeTid = TIMER_UNSET;

	next_reqid = 1;
	rotated_reqids = false;

	requestTable = new HashTable<int,GahpClient*>( 300, &hashFuncInt );
	ASSERT(requestTable);

	globus_gass_server_url = NULL;
	globus_gt2_gram_user_callback_arg = NULL;
	globus_gt2_gram_callback_func = NULL;
	globus_gt2_gram_callback_reqid = 0;
	globus_gt2_gram_callback_contact = NULL;

	globus_gt4_gram_user_callback_arg = NULL;
	globus_gt4_gram_callback_func = NULL;
	globus_gt4_gram_callback_reqid = 0;
	globus_gt4_gram_callback_contact = NULL;

	unicore_gahp_callback_func = NULL;
	unicore_gahp_callback_reqid = 0;

	my_id = strdup(id);
	binary_path = strdup(path);
	if ( args != NULL ) {
		binary_args.AppendArgsFromArgList( *args );
	}
	proxy_check_tid = TIMER_UNSET;
	master_proxy = NULL;
	is_initialized = false;
	can_cache_proxies = false;
	ProxiesByFilename = NULL;

	m_gahp_version[0] = '\0';
	m_buffer_pos = 0;
	m_buffer_end = 0;
	m_buffer = (char *)malloc( m_buffer_size );
	m_in_results = false;
}
