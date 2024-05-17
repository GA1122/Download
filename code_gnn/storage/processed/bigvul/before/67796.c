static int sig_check_netjoins(void)
{
	GSList *tmp, *next;
	int diff;
	time_t now;

	now = time(NULL);
	 
	for (tmp = joinservers; tmp != NULL; tmp = next) {
		NETJOIN_SERVER_REC *server = tmp->data;

		next = tmp->next;
		diff = now-server->last_netjoin;
		if (diff <= NETJOIN_WAIT_TIME) {
			 
			continue;
		}

                if (server->netjoins != NULL)
			print_netjoins(server, NULL);
	}

	 
	for (tmp = joinservers; tmp != NULL; tmp = next) {
		NETJOIN_SERVER_REC *server = tmp->data;

		next = tmp->next;
		diff = now-server->last_netjoin;
		if (diff >= NETJOIN_MAX_WAIT) {
			 
                        netjoin_server_remove(server);
		}
	}

	if (joinservers == NULL) {
		g_source_remove(join_tag);
		signal_remove("print starting", (SIGNAL_FUNC) sig_print_starting);
                join_tag = -1;
	}
	return 1;
}
