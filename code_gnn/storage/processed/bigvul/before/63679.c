static NICK_REC *nick_nfind(CHANNEL_REC *channel, const char *nick, int len)
{
        NICK_REC *rec;
	char *tmpnick;

	tmpnick = g_strndup(nick, len);
	rec = g_hash_table_lookup(channel->nicks, tmpnick);

	if (rec != NULL) {
		 
		while (rec->next != NULL) {
			if (g_strcmp0(rec->nick, tmpnick) == 0)
				break;
                        rec = rec->next;
		}
	}

        g_free(tmpnick);
	return rec;
}