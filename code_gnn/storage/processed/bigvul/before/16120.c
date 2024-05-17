GahpServer::Initialize( Proxy *proxy )
{
	if ( is_initialized == true ) {
		return true;
	}

	if ( m_gahp_pid == -1 ) {
		if ( Startup() == false ) {
			return false;
		}
	}

	master_proxy = new GahpProxyInfo;
	master_proxy->proxy = proxy->subject->master_proxy;
	AcquireProxy( master_proxy->proxy, (TimerHandlercpp)&GahpServer::ProxyCallback,
				  this );
	master_proxy->cached_expiration = 0;


	if ( m_commands_supported->contains_anycase( "INITIALIZE_FROM_FILE" ) ) {
		if ( command_initialize_from_file( master_proxy->proxy->proxy_filename ) == false ) {
			dprintf( D_ALWAYS, "GAHP: Failed to initialize from file\n" );
			return false;
		}
	}

	current_proxy = master_proxy;

	if ( can_cache_proxies ) {
		if ( cacheProxyFromFile( master_proxy ) == false ) {
			dprintf( D_ALWAYS, "GAHP: Failed to cache proxy from file!\n" );
			return false;
		}

		ProxiesByFilename = new HashTable<HashKey,GahpProxyInfo*>( 500,
															   &hashFunction );
		ASSERT(ProxiesByFilename);
	}

	master_proxy->cached_expiration = master_proxy->proxy->expiration_time;

	is_initialized = true;

	return true;
}
