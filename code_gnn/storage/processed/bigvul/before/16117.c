void GahpReconfig()
{
	int tmp_int;

	logGahpIo = param_boolean( "GRIDMANAGER_GAHPCLIENT_DEBUG", true );
	logGahpIoSize = param_integer( "GRIDMANAGER_GAHPCLIENT_DEBUG_SIZE", 0 );

	useXMLClassads = param_boolean( "GAHP_USE_XML_CLASSADS", false );

	tmp_int = param_integer( "GRIDMANAGER_MAX_PENDING_REQUESTS", 50 );

	GahpServer *next_server = NULL;
	GahpServer::GahpServersById.startIterations();

	while ( GahpServer::GahpServersById.iterate( next_server ) != 0 ) {
		next_server->max_pending_requests = tmp_int;
	}
}
