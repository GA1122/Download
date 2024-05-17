static void load_modules_dep(void)
{
	struct module_entry *m;
	char *colon, *tokens[2];
	parser_t *p;

	 
	p = config_open2(CONFIG_DEFAULT_DEPMOD_FILE, xfopen_for_read);

	while (G.num_unresolved_deps
	 && config_read(p, tokens, 2, 1, "# \t", PARSE_NORMAL)
	) {
		colon = last_char_is(tokens[0], ':');
		if (colon == NULL)
			continue;
		*colon = 0;

		m = get_modentry(tokens[0]);
		if (m == NULL)
			continue;

		 
		if ((m->flags & MODULE_FLAG_LOADED)
		 && !(option_mask32 & (OPT_REMOVE | OPT_SHOW_DEPS))
		) {
			DBG("skip deps of %s, it's already loaded", tokens[0]);
			continue;
		}

		m->flags |= MODULE_FLAG_FOUND_IN_MODDEP;
		if ((m->flags & MODULE_FLAG_NEED_DEPS) && (m->deps == NULL)) {
			G.num_unresolved_deps--;
			llist_add_to(&m->deps, xstrdup(tokens[0]));
			if (tokens[1])
				string_to_llist(tokens[1], &m->deps, " \t");
		} else
			DBG("skipping dep line");
	}
	config_close(p);
}
