static void nicklist_destroy(CHANNEL_REC *channel, NICK_REC *nick)
{
	signal_emit("nicklist remove", 2, channel, nick);

	if (channel->ownnick == nick)
                channel->ownnick = NULL;

         
	g_free(nick->nick);
	g_free_not_null(nick->realname);
	g_free_not_null(nick->host);
	g_free(nick);
}
