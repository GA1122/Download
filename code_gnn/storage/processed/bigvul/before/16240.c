GahpClient::~GahpClient()
{
	clear_pending();
	if ( normal_proxy != NULL ) {
		server->UnregisterProxy( normal_proxy->proxy );
	}
	if ( deleg_proxy != NULL ) {
		server->UnregisterProxy( deleg_proxy->proxy );
	}
	server->RemoveGahpClient();
}
