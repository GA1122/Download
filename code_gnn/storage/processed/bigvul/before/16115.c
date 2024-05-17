GahpServer *GahpServer::FindOrCreateGahpServer(const char *id,
											   const char *path,
											   const ArgList *args)
{
	int rc;
	GahpServer *server = NULL;

	if ( path == NULL ) {
		path = GAHPCLIENT_DEFAULT_SERVER_PATH;
	}

	rc = GahpServersById.lookup( HashKey( id ), server );
	if ( rc != 0 ) {
		server = new GahpServer( id, path, args );
		ASSERT(server);
		GahpServersById.insert( HashKey( id ), server );
	} else {
		ASSERT(server);
	}

	return server;
}
