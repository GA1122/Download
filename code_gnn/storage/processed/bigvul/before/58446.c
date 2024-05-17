server_connect (server *serv, char *hostname, int port, int no_login)
{
	int pid, read_des[2];
	session *sess = serv->server_session;

#ifdef USE_OPENSSL
	if (!serv->ctx && serv->use_ssl)
	{
		if (!(serv->ctx = _SSL_context_init (ssl_cb_info, FALSE)))
		{
			fprintf (stderr, "_SSL_context_init failed\n");
			exit (1);
		}
	}
#endif

	if (!hostname[0])
		return;

	if (port < 0)
	{
		 
		port = 6667;
#ifdef USE_OPENSSL
		if (serv->use_ssl)
			port = 6697;
#endif
	}
	port &= 0xffff;	 

	if (serv->connected || serv->connecting || serv->recondelay_tag)
		server_disconnect (sess, TRUE, -1);

	fe_progressbar_start (sess);

	EMIT_SIGNAL (XP_TE_SERVERLOOKUP, sess, hostname, NULL, NULL, NULL, 0);

	safe_strcpy (serv->servername, hostname, sizeof (serv->servername));
	 
	if (hostname != serv->hostname)
		safe_strcpy (serv->hostname, hostname, sizeof (serv->hostname));

#ifdef USE_OPENSSL
	if (serv->use_ssl)
	{
		char *cert_file;
		serv->have_cert = FALSE;

		 
		cert_file = g_strdup_printf ("%s" G_DIR_SEPARATOR_S "certs" G_DIR_SEPARATOR_S "%s.pem",
					 get_xdir (), server_get_network (serv, TRUE));
		if (SSL_CTX_use_certificate_file (serv->ctx, cert_file, SSL_FILETYPE_PEM) == 1)
		{
			if (SSL_CTX_use_PrivateKey_file (serv->ctx, cert_file, SSL_FILETYPE_PEM) == 1)
				serv->have_cert = TRUE;
		}
		else
		{
			 
			cert_file = g_build_filename (get_xdir (), "certs", "client.pem", NULL);
			if (SSL_CTX_use_certificate_file (serv->ctx, cert_file, SSL_FILETYPE_PEM) == 1)
			{
				if (SSL_CTX_use_PrivateKey_file (serv->ctx, cert_file, SSL_FILETYPE_PEM) == 1)
					serv->have_cert = TRUE;
			}
		}
		g_free (cert_file);
	}
#endif

	server_set_defaults (serv);
	serv->connecting = TRUE;
	serv->port = port;
	serv->no_login = no_login;

	fe_server_event (serv, FE_SE_CONNECTING, 0);
	fe_set_away (serv);
	server_flush_queue (serv);

#ifdef WIN32
	if (_pipe (read_des, 4096, _O_BINARY) < 0)
#else
	if (pipe (read_des) < 0)
#endif
		return;
#ifdef __EMX__  
	setmode (read_des[0], O_BINARY);
	setmode (read_des[1], O_BINARY);
#endif
	serv->childread = read_des[0];
	serv->childwrite = read_des[1];

	 
	net_sockets (&serv->sok4, &serv->sok6);
#ifdef USE_MSPROXY
	 
	if (!serv->dont_use_proxy && (serv->proxy_type == 5))
		udp_sockets (&serv->proxy_sok4, &serv->proxy_sok6);
	else
#endif
	{
		serv->proxy_sok4 = -1;
		serv->proxy_sok6 = -1;
	}

#ifdef WIN32
	CloseHandle (CreateThread (NULL, 0,
										(LPTHREAD_START_ROUTINE)server_child,
										serv, 0, (DWORD *)&pid));
#else
#ifdef LOOKUPD
	 
	rand();
#endif
	switch (pid = fork ())
	{
	case -1:
		return;

	case 0:
		 
		setuid (getuid ());
		server_child (serv);
		_exit (0);
	}
#endif
	serv->childpid = pid;
#ifdef WIN32
	serv->iotag = fe_input_add (serv->childread, FIA_READ|FIA_FD, server_read_child,
#else
	serv->iotag = fe_input_add (serv->childread, FIA_READ, server_read_child,
#endif
										 serv);
}
