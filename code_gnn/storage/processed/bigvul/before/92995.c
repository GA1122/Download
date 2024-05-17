process_redirect_pdu(STREAM s, RD_BOOL enhanced_redirect   )
{
	uint32 len;
	uint16 redirect_identifier;

	logger(Protocol, Debug, "%s()", __func__);

	 
	g_redirect = True;
	free(g_redirect_server);
	free(g_redirect_username);
	free(g_redirect_domain);
	free(g_redirect_lb_info);
	free(g_redirect_cookie);

	g_redirect_server = NULL;
	g_redirect_username = NULL;
	g_redirect_domain = NULL;
	g_redirect_lb_info = NULL;
	g_redirect_cookie = NULL;

	 
	in_uint8s(s, 2);

	 

	if (enhanced_redirect)
	{
		 
		in_uint16_le(s, redirect_identifier);
		if (redirect_identifier != 0x0400)
			logger(Protocol, Error, "unexpected data in server redirection packet");

		 
		in_uint8s(s, 2);

		 
		in_uint32_le(s, g_redirect_session_id);
	}

	 
	in_uint32_le(s, g_redirect_flags);

	if (g_redirect_flags & LB_TARGET_NET_ADDRESS)
	{
		 
		in_uint32_le(s, len);

		 
		rdp_in_unistr(s, len, &g_redirect_server, &g_redirect_server_len);
	}

	if (g_redirect_flags & LB_LOAD_BALANCE_INFO)
	{
		 
		in_uint32_le(s, g_redirect_lb_info_len);

		 
		if (g_redirect_lb_info != NULL)
			free(g_redirect_lb_info);

		g_redirect_lb_info = xmalloc(g_redirect_lb_info_len);

		 
		in_uint8p(s, g_redirect_lb_info, g_redirect_lb_info_len);
	}

	if (g_redirect_flags & LB_USERNAME)
	{
		 
		in_uint32_le(s, len);

		 
		rdp_in_unistr(s, len, &g_redirect_username, &g_redirect_username_len);
	}

	if (g_redirect_flags & LB_DOMAIN)
	{
		 
		in_uint32_le(s, len);

		 
		rdp_in_unistr(s, len, &g_redirect_domain, &g_redirect_domain_len);
	}

	if (g_redirect_flags & LB_PASSWORD)
	{
		 

		 
		in_uint32_le(s, g_redirect_cookie_len);

		 
		if (g_redirect_cookie != NULL)
			free(g_redirect_cookie);

		g_redirect_cookie = xmalloc(g_redirect_cookie_len);

		 
		in_uint8p(s, g_redirect_cookie, g_redirect_cookie_len);

		logger(Protocol, Debug, "process_redirect_pdu(), Read %d bytes redirection cookie",
		       g_redirect_cookie_len);
	}

	if (g_redirect_flags & LB_DONTSTOREUSERNAME)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_DONTSTOREUSERNAME set");
	}

	if (g_redirect_flags & LB_SMARTCARD_LOGON)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_SMARTCARD_LOGON set");
	}

	if (g_redirect_flags & LB_NOREDIRECT)
	{
		 
		g_redirect = False;
	}

	if (g_redirect_flags & LB_TARGET_FQDN)
	{
		in_uint32_le(s, len);

		 
		if (g_redirect_server)
		{
			free(g_redirect_server);
			g_redirect_server = NULL;
		}

		 
		rdp_in_unistr(s, len, &g_redirect_server, &g_redirect_server_len);
	}

	if (g_redirect_flags & LB_TARGET_NETBIOS)
	{
		logger(Protocol, Warning, "process_redirect_pdu(), unhandled LB_TARGET_NETBIOS");
	}

	if (g_redirect_flags & LB_TARGET_NET_ADDRESSES)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_TARGET_NET_ADDRESSES");
	}

	if (g_redirect_flags & LB_CLIENT_TSV_URL)
	{
		logger(Protocol, Warning, "process_redirect_pdu(), unhandled LB_CLIENT_TSV_URL");
	}

	if (g_redirect_flags & LB_SERVER_TSV_CAPABLE)
	{
		logger(Protocol, Warning, "process_redirect_pdu(), unhandled LB_SERVER_TSV_URL");
	}

	if (g_redirect_flags & LB_PASSWORD_IS_PK_ENCRYPTED)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_PASSWORD_IS_PK_ENCRYPTED ");
	}

	if (g_redirect_flags & LB_REDIRECTION_GUID)
	{
		logger(Protocol, Warning, "process_redirect_pdu(), unhandled LB_REDIRECTION_GUID ");
	}

	if (g_redirect_flags & LB_TARGET_CERTIFICATE)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_TARGET_CERTIFICATE");
	}

	return g_redirect;
}
