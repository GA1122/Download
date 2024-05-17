static void server_connect_callback_readpipe(SERVER_REC *server)
{
	RESOLVED_IP_REC iprec;
        IPADDR *ip;
	const char *errormsg;
	char *servername = NULL;

	g_source_remove(server->connect_tag);
	server->connect_tag = -1;

	net_gethostbyname_return(server->connect_pipe[0], &iprec);

	g_io_channel_close(server->connect_pipe[0]);
	g_io_channel_unref(server->connect_pipe[0]);
	g_io_channel_close(server->connect_pipe[1]);
	g_io_channel_unref(server->connect_pipe[1]);

	server->connect_pipe[0] = NULL;
	server->connect_pipe[1] = NULL;

	 
	if (iprec.error != 0) {
                 
		ip = NULL;
	} else if (server->connrec->family == AF_INET) {
		 
		ip = iprec.ip4.family == 0 ? NULL : &iprec.ip4;
		servername = iprec.host4;
	} else if (server->connrec->family == AF_INET6) {
		 
		ip = iprec.ip6.family == 0 ? NULL : &iprec.ip6;
		servername = iprec.host6;
	} else {
		 
		if (iprec.ip4.family == 0 ||
		    (iprec.ip6.family != 0 &&
		     settings_get_bool("resolve_prefer_ipv6"))) {
			ip = &iprec.ip6;
			servername = iprec.host6;
		} else {
			ip = &iprec.ip4;
			servername = iprec.host4;
		}
	}

	if (ip != NULL) {
		 
		if (servername) {
			g_free(server->connrec->address);
			server->connrec->address = g_strdup(servername);
		}
		server_real_connect(server, ip, NULL);
		errormsg = NULL;
	} else {
		if (iprec.error == 0 || net_hosterror_notfound(iprec.error)) {
			 
			server->dns_error = TRUE;
		}

		if (iprec.error == 0) {
			 
			errormsg = server->connrec->family == AF_INET ?
				"IPv4 address not found for host" :
				"IPv6 address not found for host";
		} else {
			 
			errormsg = iprec.errorstr != NULL ? iprec.errorstr :
				"Host lookup failed";
		}

		server->connection_lost = TRUE;
		server_connect_failed(server, errormsg);
	}

	g_free(iprec.errorstr);
	g_free(iprec.host4);
	g_free(iprec.host6);
}
