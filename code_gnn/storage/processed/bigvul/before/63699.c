void nicklist_set_own(CHANNEL_REC *channel, NICK_REC *nick)
{
	NICK_REC *first, *next;

        channel->ownnick = nick;

	 
	first = g_hash_table_lookup(channel->nicks, nick->nick);
	if (first->next == NULL)
		return;

	next = nick->next;
	nick->next = first;

	while (first->next != nick)
                first = first->next;
	first->next = next;

        g_hash_table_insert(channel->nicks, nick->nick, nick);
}
