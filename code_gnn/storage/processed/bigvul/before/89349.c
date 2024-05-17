static int parse_label_menu(char **c, struct pxe_menu *cfg,
			    struct pxe_label *label)
{
	struct token t;
	char *s;

	s = *c;

	get_token(c, &t, L_KEYWORD);

	switch (t.type) {
	case T_DEFAULT:
		if (!cfg->default_label)
			cfg->default_label = strdup(label->name);

		if (!cfg->default_label)
			return -ENOMEM;

		break;
	case T_LABEL:
		parse_sliteral(c, &label->menu);
		break;
	default:
		printf("Ignoring malformed menu command: %.*s\n",
		       (int)(*c - s), s);
	}

	eol_or_eof(c);

	return 0;
}
