int modprobe_main(int argc UNUSED_PARAM, char **argv)
{
	int rc;
	unsigned opt;
	struct module_entry *me;

	INIT_G();

	IF_LONG_OPTS(applet_long_options = modprobe_longopts;)
	opt_complementary = MODPROBE_COMPLEMENTARY;
	opt = getopt32(argv, INSMOD_OPTS MODPROBE_OPTS INSMOD_ARGS);
	argv += optind;

	 
	xchdir(CONFIG_DEFAULT_MODULES_DIR);
	uname(&G.uts);
	xchdir(G.uts.release);

	if (opt & OPT_LIST_ONLY) {
		int i;
		char name[MODULE_NAME_LEN];
		char *colon, *tokens[2];
		parser_t *p = config_open2(CONFIG_DEFAULT_DEPMOD_FILE, xfopen_for_read);

		for (i = 0; argv[i]; i++)
			replace(argv[i], '-', '_');

		while (config_read(p, tokens, 2, 1, "# \t", PARSE_NORMAL)) {
			colon = last_char_is(tokens[0], ':');
			if (!colon)
				continue;
			*colon = '\0';
			filename2modname(tokens[0], name);
			if (!argv[0])
				puts(tokens[0]);
			else {
				for (i = 0; argv[i]; i++) {
					if (fnmatch(argv[i], name, 0) == 0) {
						puts(tokens[0]);
					}
				}
			}
		}
		return EXIT_SUCCESS;
	}

	 
	if (opt & INSMOD_OPT_SYSLOG)
		logmode = LOGMODE_SYSLOG;

	if (!argv[0]) {
		if (opt & OPT_REMOVE) {
			 
			if (bb_delete_module(NULL, O_NONBLOCK | O_EXCL) != 0)
				bb_perror_msg_and_die("rmmod");
		}
		return EXIT_SUCCESS;
	}

	 
	{
		char *s;
		parser_t *parser = config_open2("/proc/modules", fopen_for_read);
		while (config_read(parser, &s, 1, 1, "# \t", PARSE_NORMAL & ~PARSE_GREEDY))
			get_or_add_modentry(s)->flags |= MODULE_FLAG_LOADED;
		config_close(parser);
	}

	if (opt & (OPT_INSERT_ALL | OPT_REMOVE)) {
		 
		do {
			DBG("adding module %s", *argv);
			add_probe(*argv++);
		} while (*argv);
	} else {
		 
		DBG("probing just module %s", *argv);
		add_probe(argv[0]);
		G.cmdline_mopts = parse_cmdline_module_options(argv,   1);
	}

	 
	if (G.probes == NULL)
		return EXIT_SUCCESS;

	read_config("/etc/modprobe.conf");
	read_config("/etc/modprobe.d");
	if (ENABLE_FEATURE_MODUTILS_SYMBOLS && G.need_symbols)
		read_config("modules.symbols");
	load_modules_dep();
	if (ENABLE_FEATURE_MODUTILS_ALIAS && G.num_unresolved_deps) {
		read_config("modules.alias");
		load_modules_dep();
	}

	rc = 0;
	while ((me = llist_pop(&G.probes)) != NULL) {
		if (me->realnames == NULL) {
			DBG("probing by module name");
			 
			if (!(opt & OPT_BLACKLIST)
			 || !(me->flags & MODULE_FLAG_BLACKLISTED)
			) {
				rc |= do_modprobe(me);
			}
			continue;
		}

		 
		do {
			char *realname = llist_pop(&me->realnames);
			struct module_entry *m2;

			DBG("probing alias %s by realname %s", me->modname, realname);
			m2 = get_or_add_modentry(realname);
			if (!(m2->flags & MODULE_FLAG_BLACKLISTED)
			 && (!(m2->flags & MODULE_FLAG_LOADED)
			    || (opt & (OPT_REMOVE | OPT_SHOW_DEPS)))
			) {
				rc |= do_modprobe(m2);
			}
			free(realname);
		} while (me->realnames != NULL);
	}

	return (rc != 0);
}
