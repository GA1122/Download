static void make_default_cur_rule(void)
{
	memset(&G.cur_rule, 0, sizeof(G.cur_rule));
	G.cur_rule.maj = -1;  
	G.cur_rule.mode = 0660;
}
