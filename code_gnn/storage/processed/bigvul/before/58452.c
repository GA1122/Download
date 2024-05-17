server_get_network (server *serv, gboolean fallback)
{
	 
	if (serv->network)
		return ((ircnet *)serv->network)->name;

	 
	if (serv->server_session && *serv->server_session->channel)
		return serv->server_session->channel;

	if (fallback)
		return serv->servername;

	return NULL;
}
