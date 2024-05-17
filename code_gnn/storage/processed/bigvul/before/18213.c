int server_start_connect(SERVER_REC *server)
{
	const char *connect_address;
        int fd[2];

	g_return_val_if_fail(server != NULL, FALSE);
	if (!server->connrec->unix_socket && server->connrec->port <= 0)
		return FALSE;

	server->rawlog = rawlog_create();

	if (server->connrec->connect_handle != NULL) {
		 
		GIOChannel *handle = server->connrec->connect_handle;

		server->connrec->connect_handle = NULL;
		server->handle = net_sendbuffer_create(handle, 0);
		server_connect_finished(server);
	} else if (server->connrec->unix_socket) {
		 
		server_real_connect(server, NULL, server->connrec->address);
	} else {
		 
		if (pipe(fd) != 0) {
			g_warning("server_connect(): pipe() failed.");
			g_free(server->tag);
			g_free(server->nick);
			return FALSE;
		}

		server->connect_pipe[0] = g_io_channel_unix_new(fd[0]);
		server->connect_pipe[1] = g_io_channel_unix_new(fd[1]);

		connect_address = server->connrec->proxy != NULL ?
			server->connrec->proxy : server->connrec->address;
		server->connect_pid =
			net_gethostbyname_nonblock(connect_address,
						   server->connect_pipe[1],
						   settings_get_bool("resolve_reverse_lookup"));
		server->connect_tag =
			g_input_add(server->connect_pipe[0], G_INPUT_READ,
				    (GInputFunction)
				    server_connect_callback_readpipe,
				    server);

		lookup_servers = g_slist_append(lookup_servers, server);

		signal_emit("server looking", 1, server);
	}
	return TRUE;
}