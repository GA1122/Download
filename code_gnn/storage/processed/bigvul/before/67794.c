static void print_netjoins(NETJOIN_SERVER_REC *server, const char *filter_channel)
{
	TEMP_PRINT_REC *temp;
	GHashTable *channels;
	GSList *tmp, *tmp2, *next, *next2, *old;

	g_return_if_fail(server != NULL);

	printing_joins = TRUE;

	 
	channels = g_hash_table_new((GHashFunc) g_istr_hash,
				    (GCompareFunc) g_istr_equal);
	for (tmp = server->netjoins; tmp != NULL; tmp = next) {
		NETJOIN_REC *rec = tmp->data;

		next = g_slist_next(tmp);

		for (tmp2 = rec->now_channels; tmp2 != NULL; tmp2 = next2) {
			char *channel = tmp2->data;
			char *realchannel = channel + 1;

			next2 = g_slist_next(tmp2);

			 
			if (filter_channel != NULL &&
			    strcasecmp(realchannel, filter_channel) != 0)
				continue;

			temp = g_hash_table_lookup(channels, realchannel);
			if (temp == NULL) {
				temp = g_new0(TEMP_PRINT_REC, 1);
				temp->nicks = g_string_new(NULL);
				g_hash_table_insert(channels,
						    g_strdup(realchannel),
						    temp);
			}

			temp->count++;
			if (temp->count <= netjoin_max_nicks) {
				if (*channel != ' ')
					g_string_append_c(temp->nicks,
							  *channel);
				g_string_append_printf(temp->nicks, "%s, ",
						  rec->nick);
			}

			 
			old = gslist_find_icase_string(rec->old_channels,
						       realchannel);
			if (old != NULL) {
				void *data = old->data;
				rec->old_channels =
					g_slist_remove(rec->old_channels, data);
				g_free(data);
			}

			 
			rec->now_channels = g_slist_delete_link(rec->now_channels, tmp2);
			g_free(channel);
		}

		if (rec->old_channels == NULL)
                        netjoin_remove(server, rec);
	}

	g_hash_table_foreach(channels, (GHFunc) print_channel_netjoins,
			     server);
	g_hash_table_destroy(channels);

	if (server->netjoins == NULL)
		netjoin_server_remove(server);

	printing_joins = FALSE;
}