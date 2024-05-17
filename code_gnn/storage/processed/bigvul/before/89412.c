static void event_kill(IRC_SERVER_REC *server, const char *data,
		       const char *nick, const char *addr)
{
	if (addr != NULL && !server->nick_collision) {
		 
		server->no_reconnect = TRUE;
	}
}
