static void msg_mode(IRC_SERVER_REC *server, const char *channel,
		     const char *sender, const char *addr, const char *data)
{
	NETJOIN_REC *rec;
	char *params, *mode, *nicks;
	char **nicklist, **nick, type, prefix;
	int show;

	g_return_if_fail(data != NULL);
	if (!server_ischannel(SERVER(server), channel) || addr != NULL)
		return;

	params = event_get_params(data, 2 | PARAM_FLAG_GETREST,
				  &mode, &nicks);

	 
	nick = nicklist = g_strsplit(nicks, " ", -1);

	type = '+'; show = FALSE;
	for (; *mode != '\0'; mode++) {
		if (*mode == '+' || *mode == '-') {
			type = *mode;
			continue;
		}

		if (*nick != NULL && GET_MODE_PREFIX(server, *mode)) {
                         
			rec = netjoin_find(server, *nick);
			prefix = GET_MODE_PREFIX(server, *mode);
			if (rec == NULL || type != '+' || prefix == '\0' ||
			    !netjoin_set_nickmode(server, rec, channel, prefix))
				show = TRUE;
                        nick++;
		} else {
			if (HAS_MODE_ARG(server, type, *mode) && *nick != NULL)
				nick++;
			show = TRUE;
		}
	}

	if (!show) signal_stop();

	g_strfreev(nicklist);
	g_free(params);
}