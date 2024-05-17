static void event_nick_collision(IRC_SERVER_REC *server, const char *data)
{
	time_t new_connect;

	if (!IS_IRC_SERVER(server))
		return;

	 

	new_connect = server->connect_time+10 -
		settings_get_time("server_reconnect_time")/1000;
	if (server->connect_time > new_connect)
		server->connect_time = new_connect;

        server->nick_collision = TRUE;
}
