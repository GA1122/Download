static void nick_hash_add(CHANNEL_REC *channel, NICK_REC *nick)
{
	NICK_REC *list;

	nick->next = NULL;

	list = g_hash_table_lookup(channel->nicks, nick->nick);
        if (list == NULL)
		g_hash_table_insert(channel->nicks, nick->nick, nick);
	else {
                 
		while (list->next != NULL)
			list = list->next;
		list->next = nick;
	}

	if (nick == channel->ownnick) {
                 
		nicklist_set_own(channel, nick);
	}
}
