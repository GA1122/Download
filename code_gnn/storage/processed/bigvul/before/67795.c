static void read_settings(void)
{
	int old_hide;

        old_hide = hide_netsplit_quits;
	hide_netsplit_quits = settings_get_bool("hide_netsplit_quits");
	netjoin_max_nicks = settings_get_int("netjoin_max_nicks");

	if (old_hide && !hide_netsplit_quits) {
		signal_remove("message quit", (SIGNAL_FUNC) msg_quit);
		signal_remove("message join", (SIGNAL_FUNC) msg_join);
		signal_remove("message irc mode", (SIGNAL_FUNC) msg_mode);
	} else if (!old_hide && hide_netsplit_quits) {
		signal_add("message quit", (SIGNAL_FUNC) msg_quit);
		signal_add("message join", (SIGNAL_FUNC) msg_join);
		signal_add("message irc mode", (SIGNAL_FUNC) msg_mode);
 	}
 }