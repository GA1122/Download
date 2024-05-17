GahpServer::UnregisterProxy( Proxy *proxy )
{
	int rc;
	GahpProxyInfo *gahp_proxy = NULL;

	if ( ProxiesByFilename == NULL || proxy == NULL ||
		 can_cache_proxies == false ) {

		return;
	}

	if ( master_proxy != NULL && proxy == master_proxy->proxy ) {
		master_proxy->num_references--;
		return;
	}

	rc = ProxiesByFilename->lookup( HashKey( proxy->proxy_filename ),
									gahp_proxy );

	if ( rc != 0 ) {
		dprintf( D_ALWAYS, "GahpServer::UnregisterProxy() called with unknown proxy %s\n", proxy->proxy_filename );
		return;
	}

	gahp_proxy->num_references--;

	if ( gahp_proxy->num_references == 0 ) {
		ProxiesByFilename->remove( HashKey( gahp_proxy->proxy->proxy_filename ) );
		uncacheProxy( gahp_proxy );
		ReleaseProxy( gahp_proxy->proxy, (TimerHandlercpp)&GahpServer::ProxyCallback,
					  this );
		delete gahp_proxy;
	}
}
