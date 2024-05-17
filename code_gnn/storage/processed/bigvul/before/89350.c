static int parse_menu(cmd_tbl_t *cmdtp, char **c, struct pxe_menu *cfg,
		      unsigned long base, int nest_level)
{
	struct token t;
	char *s = *c;
	int err = 0;

	get_token(c, &t, L_KEYWORD);

	switch (t.type) {
	case T_TITLE:
		err = parse_sliteral(c, &cfg->title);

		break;

	case T_INCLUDE:
		err = handle_include(cmdtp, c, base, cfg, nest_level + 1);
		break;

	case T_BACKGROUND:
		err = parse_sliteral(c, &cfg->bmp);
		break;

	default:
		printf("Ignoring malformed menu command: %.*s\n",
		       (int)(*c - s), s);
	}

	if (err < 0)
		return err;

	eol_or_eof(c);

	return 1;
}
