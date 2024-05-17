static void parse_next_rule(void)
{
	 
	while (1) {
		char *tokens[4];
		char *val;

		 
		if (!config_read(G.parser, tokens, 4, 3, "# \t", PARSE_NORMAL & ~PARSE_EOL_COMMENTS))
			break;

		 
		dbg("token1:'%s'", tokens[1]);

		 
		val = tokens[0];
		G.cur_rule.keep_matching = ('-' == val[0]);
		val += G.cur_rule.keep_matching;  
		if (val[0] == '@') {
			 
			 
			int sc = sscanf(val, "@%u,%u-%u", &G.cur_rule.maj, &G.cur_rule.min0, &G.cur_rule.min1);
			if (sc < 2 || G.cur_rule.maj < 0) {
				bb_error_msg("bad @maj,min on line %d", G.parser->lineno);
				goto next_rule;
			}
			if (sc == 2)
				G.cur_rule.min1 = G.cur_rule.min0;
		} else {
			if (val[0] == '$') {
				char *eq = strchr(++val, '=');
				if (!eq) {
					bb_error_msg("bad $envvar=regex on line %d", G.parser->lineno);
					goto next_rule;
				}
				G.cur_rule.envvar = xstrndup(val, eq - val);
				val = eq + 1;
			}
			xregcomp(&G.cur_rule.match, val, REG_EXTENDED);
			G.cur_rule.regex_compiled = 1;
		}

		 
		if (get_uidgid(&G.cur_rule.ugid, tokens[1],   1) == 0) {
			bb_error_msg("unknown user/group '%s' on line %d", tokens[1], G.parser->lineno);
			goto next_rule;
		}

		 
		bb_parse_mode(tokens[2], &G.cur_rule.mode);

		 
		val = tokens[3];
		if (ENABLE_FEATURE_MDEV_RENAME && val && strchr(">=!", val[0])) {
			char *s = skip_non_whitespace(val);
			G.cur_rule.ren_mov = xstrndup(val, s - val);
			val = skip_whitespace(s);
		}

		if (ENABLE_FEATURE_MDEV_EXEC && val && val[0]) {
			const char *s = "$@*";
			const char *s2 = strchr(s, val[0]);
			if (!s2) {
				bb_error_msg("bad line %u", G.parser->lineno);
				goto next_rule;
			}
			IF_FEATURE_MDEV_EXEC(G.cur_rule.r_cmd = xstrdup(val);)
		}

		return;
 next_rule:
		clean_up_cur_rule();
	}  

	dbg("config_close(G.parser)");
	config_close(G.parser);
	G.parser = NULL;

	return;
}
