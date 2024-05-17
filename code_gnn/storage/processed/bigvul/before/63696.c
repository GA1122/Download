static void nicklist_rename_list(SERVER_REC *server, void *new_nick_id,
				 const char *old_nick, const char *new_nick,
				 GSList *nicks)
{
	CHANNEL_REC *channel;
	NICK_REC *nickrec;
	GSList *tmp;

	for (tmp = nicks; tmp != NULL; tmp = tmp->next->next) {
		channel = tmp->data;
		nickrec = tmp->next->data;

		 
                nick_hash_remove(channel, nickrec);

		if (new_nick_id != NULL)
			nickrec->unique_id = new_nick_id;

		g_free(nickrec->nick);
		nickrec->nick = g_strdup(new_nick);

		 
                nick_hash_add(channel, nickrec);

		signal_emit("nicklist changed", 3, channel, nickrec, old_nick);
	}
	g_slist_free(nicks);
}