static void clean_up_cur_rule(void)
{
	free(G.cur_rule.envvar);
	if (G.cur_rule.regex_compiled)
		regfree(&G.cur_rule.match);
	free(G.cur_rule.ren_mov);
	IF_FEATURE_MDEV_EXEC(free(G.cur_rule.r_cmd);)
	make_default_cur_rule();
}
