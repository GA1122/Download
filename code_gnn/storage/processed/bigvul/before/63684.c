NICK_REC *nicklist_find_mask(CHANNEL_REC *channel, const char *mask)
{
	NICK_REC *nickrec;
	char *nick, *host;

	g_return_val_if_fail(IS_CHANNEL(channel), NULL);
	g_return_val_if_fail(mask != NULL, NULL);

	nick = g_strdup(mask);
	host = strchr(nick, '!');
	if (host != NULL) *host++ = '\0';

	if (strchr(nick, '*') || strchr(nick, '?')) {
		g_free(nick);
		return nicklist_find_wildcards(channel, mask);
	}

	nickrec = g_hash_table_lookup(channel->nicks, nick);

	if (host != NULL) {
		while (nickrec != NULL) {
			if (nickrec->host != NULL &&
			    match_wildcards(host, nickrec->host))
				break;  
			nickrec = nickrec->next;
		}
	}
	g_free(nick);
	return nickrec;
}