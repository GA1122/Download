static const struct rule *next_rule(void)
{
	struct rule *rule;

	 
	if (!G.parser && G.filename) {
		dbg("config_open('%s')", G.filename);
		G.parser = config_open2(G.filename, fopen_for_read);
		G.filename = NULL;
	}

	if (G.rule_vec) {
		 
		 
		if (G.rule_vec[G.rule_idx]) {
			dbg("< G.rule_vec[G.rule_idx:%d]=%p", G.rule_idx, G.rule_vec[G.rule_idx]);
			return G.rule_vec[G.rule_idx++];
		}
		make_default_cur_rule();
	} else {
		 
		clean_up_cur_rule();
	}

	 
	rule = &G.cur_rule;
	if (G.parser) {
		parse_next_rule();
		if (G.rule_vec) {  
			rule = memcpy(xmalloc(sizeof(G.cur_rule)), &G.cur_rule, sizeof(G.cur_rule));
			G.rule_vec = xrealloc_vector(G.rule_vec, 4, G.rule_idx);
			G.rule_vec[G.rule_idx++] = rule;
			dbg("> G.rule_vec[G.rule_idx:%d]=%p", G.rule_idx, G.rule_vec[G.rule_idx]);
		}
	}

	return rule;
}
