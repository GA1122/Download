static void parse_ttymodes(Ssh ssh,
                           void (*do_mode)(void *data,
                                           const struct ssh_ttymode *mode,
                                           char *val),
			   void *data)
{
    int i;
    const struct ssh_ttymode *mode;
    char *val;
    char default_val[2];

    strcpy(default_val, "A");

    for (i = 0; i < lenof(ssh_ttymodes); i++) {
        mode = ssh_ttymodes + i;
        val = conf_get_str_str_opt(ssh->conf, CONF_ttymodes, mode->mode);
        if (!val)
            val = default_val;

	 
	if (val[0] == 'A') {
	    val = get_ttymode(ssh->frontend, mode->mode);
	    if (val) {
		do_mode(data, mode, val);
		sfree(val);
	    }
	} else
            do_mode(data, mode, val + 1);               
    }
}
