rdp_connect(char *server, uint32 flags, char *domain, char *password,
	    char *command, char *directory, RD_BOOL reconnect)
{
	RD_BOOL deactivated = False;
	uint32 ext_disc_reason = 0;

	if (!sec_connect(server, g_username, domain, password, reconnect))
		return False;

	rdp_send_client_info_pdu(flags, domain, g_username, password, command, directory);

	 
	while (!g_rdp_shareid)
	{
		if (g_network_error)
			return False;

		if (!rdp_loop(&deactivated, &ext_disc_reason))
			return False;

		if (g_redirect)
			return True;
	}
	return True;
}