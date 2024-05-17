void nicklist_insert(CHANNEL_REC *channel, NICK_REC *nick)
{
	 

	nick->type = module_get_uniq_id("NICK", 0);
        nick->chat_type = channel->chat_type;

        nick_hash_add(channel, nick);
	signal_emit("nicklist new", 2, channel, nick);
}