static bool handle_client_work(PgSocket *client, PktHdr *pkt)
{
	SBuf *sbuf = &client->sbuf;

	switch (pkt->type) {

	 
	case 'Q':		 
		if (cf_disable_pqexec) {
			slog_error(client, "Client used 'Q' packet type.");
			disconnect_client(client, true, "PQexec disallowed");
			return false;
		}
	case 'F':		 

	 
	case 'H':		 
	case 'S':		 

	 
	case 'c':		 
	case 'f':		 

	 
	case 'P':		 
	case 'E':		 
	case 'C':		 
	case 'B':		 
	case 'D':		 
	case 'd':		 

		 
		if (!client->query_start) {
			client->pool->stats.request_count++;
			client->query_start = get_cached_time();
		}

		if (client->pool->db->admin)
			return admin_handle_client(client, pkt);

		 
		if (!find_server(client))
			return false;

		client->pool->stats.client_bytes += pkt->len;

		 
		client->link->ready = false;
		client->link->idle_tx = false;

		 
		sbuf_prepare_send(sbuf, &client->link->sbuf, pkt->len);
		break;

	 
	default:
		slog_error(client, "unknown pkt from client: %d/0x%x", pkt->type, pkt->type);
		disconnect_client(client, true, "unknown pkt");
		return false;
	case 'X':  
		disconnect_client(client, false, "client close request");
		return false;
	}
	return true;
}
