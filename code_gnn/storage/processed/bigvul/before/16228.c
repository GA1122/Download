GahpClient::setNormalProxy( Proxy *proxy )
{
	if ( !server->can_cache_proxies ) {
		return;
	}
	if ( normal_proxy != NULL && proxy == normal_proxy->proxy ) {
		return;
	}
	if ( normal_proxy != NULL ) {
		server->UnregisterProxy( normal_proxy->proxy );
	}
	GahpProxyInfo *gahp_proxy = server->RegisterProxy( proxy );
	ASSERT(gahp_proxy);
	normal_proxy = gahp_proxy;
}
