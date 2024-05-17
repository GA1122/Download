GahpServer::doProxyCheck()
{
	proxy_check_tid = TIMER_UNSET;

	if ( m_gahp_pid == -1 ) {
		return;
	}

	GahpProxyInfo *next_proxy;

	if ( ProxiesByFilename ) {
		ProxiesByFilename->startIterations();
		while ( ProxiesByFilename->iterate( next_proxy ) != 0 ) {

			if ( next_proxy->proxy->expiration_time >
				 next_proxy->cached_expiration ) {

				if ( cacheProxyFromFile( next_proxy ) == false ) {
					EXCEPT( "Failed to refresh proxy!" );
				}
				next_proxy->cached_expiration = next_proxy->proxy->expiration_time;

			}

		}
	}

	if ( master_proxy->proxy->expiration_time >
		 master_proxy->cached_expiration ) {

		static const char *command = "REFRESH_PROXY_FROM_FILE";
		if ( m_commands_supported->contains_anycase( command ) ) {
			if ( command_initialize_from_file(
										master_proxy->proxy->proxy_filename,
										command) == false ) {
				EXCEPT( "Failed to refresh proxy!" );
			}
		}

		if ( can_cache_proxies ) {
			if ( cacheProxyFromFile( master_proxy ) == false ) {
				EXCEPT( "Failed to refresh proxy!" );
			}
		}

		master_proxy->cached_expiration = master_proxy->proxy->expiration_time;
	}
}
