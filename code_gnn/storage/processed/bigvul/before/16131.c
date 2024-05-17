GahpServer::cacheProxyFromFile( GahpProxyInfo *new_proxy )
{
	if ( command_cache_proxy_from_file( new_proxy ) == false ) {
		return false;
	}

	if ( new_proxy == current_proxy ) {
		command_use_cached_proxy( new_proxy );
	}

	return true;
}
