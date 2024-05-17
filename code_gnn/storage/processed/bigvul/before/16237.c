GahpServer::useCachedProxy( GahpProxyInfo *new_proxy, bool force )
{
	if ( new_proxy == NULL ) {
		new_proxy = current_proxy;
	}

	if ( new_proxy->cached_expiration != new_proxy->proxy->expiration_time ) {
		if ( command_cache_proxy_from_file( new_proxy ) == false ) {
			EXCEPT( "Failed to recache proxy!" );
		}
		new_proxy->cached_expiration = new_proxy->proxy->expiration_time;
		force = true;
	}

	if ( force == false && new_proxy == current_proxy ) {
		return true;
	}

	if ( command_use_cached_proxy( new_proxy ) == false ) {
		return false;
	}

	current_proxy = new_proxy;

	return true;
}
