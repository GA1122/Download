static void init_userinfo(void)
{
	unsigned int changed;
	const char *set, *nick, *user_name, *str;

	changed = 0;
	 
        set = settings_get_str("real_name");
	if (set == NULL || *set == '\0') {
		str = g_getenv("IRCNAME");
		settings_set_str("real_name",
				 str != NULL ? str : g_get_real_name());
		changed |= USER_SETTINGS_REAL_NAME;
	}

	 
        user_name = settings_get_str("user_name");
	if (user_name == NULL || *user_name == '\0') {
		str = g_getenv("IRCUSER");
		settings_set_str("user_name",
				 str != NULL ? str : g_get_user_name());

		user_name = settings_get_str("user_name");
		changed |= USER_SETTINGS_USER_NAME;
	}

	 
        nick = settings_get_str("nick");
	if (nick == NULL || *nick == '\0') {
		str = g_getenv("IRCNICK");
		settings_set_str("nick", str != NULL ? str : user_name);

		nick = settings_get_str("nick");
		changed |= USER_SETTINGS_NICK;
	}

	 
        set = settings_get_str("hostname");
	if (set == NULL || *set == '\0') {
		str = g_getenv("IRCHOST");
		if (str != NULL) {
			settings_set_str("hostname", str);
			changed |= USER_SETTINGS_HOSTNAME;
		}
	}

	signal_emit("irssi init userinfo changed", 1, GUINT_TO_POINTER(changed));
}