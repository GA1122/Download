GahpServer::~GahpServer()
{
	if ( my_id != NULL ) {
		GahpServersById.remove( HashKey( my_id ) );
	}
	if ( m_deleteMeTid != TIMER_UNSET ) {
		daemonCore->Cancel_Timer( m_deleteMeTid );
	}
	free( m_buffer );
	delete m_commands_supported;
	if ( globus_gass_server_url != NULL ) {
		free( globus_gass_server_url );
	}
	if ( globus_gt2_gram_callback_contact != NULL ) {
		free( globus_gt2_gram_callback_contact );
	}
	if ( globus_gt4_gram_callback_contact != NULL ) {
		free( globus_gt4_gram_callback_contact );
	}
	if ( my_id != NULL ) {
		free(my_id);
	}
	if ( binary_path != NULL ) {
		free(binary_path);
	}
	if ( m_gahp_readfd != -1 ) {
		daemonCore->Close_Pipe( m_gahp_readfd );
	}
	if ( m_gahp_writefd != -1 ) {
		daemonCore->Close_Pipe( m_gahp_writefd );
	}
	if ( m_gahp_errorfd != -1 ) {
		daemonCore->Close_Pipe( m_gahp_errorfd );
	}
	if ( poll_tid != -1 ) {
		daemonCore->Cancel_Timer( poll_tid );
	}
	if ( master_proxy != NULL ) {
		ReleaseProxy( master_proxy->proxy, (TimerHandlercpp)&GahpServer::ProxyCallback,
					  this );
		delete master_proxy;
	}
	if ( proxy_check_tid != TIMER_UNSET ) {
		daemonCore->Cancel_Timer( proxy_check_tid );
	}
	if ( ProxiesByFilename != NULL ) {
		GahpProxyInfo *gahp_proxy;

		ProxiesByFilename->startIterations();
		while ( ProxiesByFilename->iterate( gahp_proxy ) != 0 ) {
			ReleaseProxy( gahp_proxy->proxy,
						  (TimerHandlercpp)&GahpServer::ProxyCallback, this );
			delete gahp_proxy;
		}

		delete ProxiesByFilename;
	}
	if ( requestTable != NULL ) {
		delete requestTable;
	}
}
