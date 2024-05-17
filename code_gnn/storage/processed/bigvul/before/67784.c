static void msg_join(IRC_SERVER_REC *server, const char *channel,
		     const char *nick, const char *address)
{
	NETSPLIT_REC *split;
	NETJOIN_REC *netjoin;
	GSList *channels;
	int rejoin = 1;

	if (!IS_IRC_SERVER(server))
		return;

	if (ignore_check(SERVER(server), nick, address,
			 channel, NULL, MSGLEVEL_JOINS))
		return;

	split = netsplit_find(server, nick, address);
	netjoin = netjoin_find(server, nick);
	if (split == NULL && netjoin == NULL)
                return;

	 
	if (netjoin != NULL) {
		if (!gslist_find_icase_string(netjoin->old_channels, channel))
			return;
	} else {
		channels = split->channels;
		while (channels != NULL) {
			NETSPLIT_CHAN_REC *schannel = channels->data;

			if (!strcasecmp(schannel->name, channel))
				break;
			channels = channels->next;
		}
		 
		if (channels == NULL)
			rejoin = 0;
	}

	if (join_tag == -1) {
		join_tag = g_timeout_add(1000, (GSourceFunc)
					 sig_check_netjoins, NULL);
		signal_add("print starting", (SIGNAL_FUNC) sig_print_starting);
	}

	if (netjoin == NULL)
		netjoin = netjoin_add(server, nick, split->channels);

	if (rejoin)
	{
		netjoin->now_channels = g_slist_append(netjoin->now_channels,
						       g_strconcat(" ", channel, NULL));
		signal_stop();
	}
}