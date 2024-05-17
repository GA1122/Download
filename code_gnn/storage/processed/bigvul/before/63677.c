int nick_match_msg(CHANNEL_REC *channel, const char *msg, const char *nick)
{
	const char *msgstart, *orignick;
	int len, fullmatch;

	g_return_val_if_fail(nick != NULL, FALSE);
	g_return_val_if_fail(msg != NULL, FALSE);

	if (channel != NULL && channel->server->nick_match_msg != NULL)
		return channel->server->nick_match_msg(msg, nick);

	 
	len = strlen(nick);
	if (g_ascii_strncasecmp(msg, nick, len) == 0 &&
	    !isalnumhigh((int) msg[len]))
		return TRUE;

	orignick = nick;
	for (;;) {
		nick = orignick;
		msgstart = msg;
                fullmatch = TRUE;

		 
		while (*nick != '\0' && *msg != '\0') {
			if (i_toupper(*nick) == i_toupper(*msg)) {
				 
				msg++;
			} else if (i_isalnum(*msg) && !i_isalnum(*nick)) {
				 
                                fullmatch = FALSE;
			} else
				break;

			nick++;
		}

		if (msg != msgstart && !isalnumhigh(*msg)) {
			 
			if (*nick != '\0') {
				 
                                fullmatch = FALSE;
				while (*nick != '\0' && !i_isalnum(*nick))
					nick++;
			}

			if (*nick == '\0') {
				 
                                break;
			}
		}

		 
		while (*msg != '\0' && *msg != ' ' && *msg != ',') msg++;

		if (*msg != ',') {
                        nick = orignick;
			break;
		}

                msg++;
	}

	if (*nick != '\0')
		return FALSE;  

	if (fullmatch)
		return TRUE;  

	if (channel != NULL) {
		 
		return nick_nfind(channel, msgstart, (int) (msg-msgstart)) == NULL;
	} else {
		return TRUE;
	}
}
