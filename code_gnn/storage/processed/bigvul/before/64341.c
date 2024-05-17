static void aliascmd(RCore *core, const char *str) {
	switch (str[0]) {
	case '-':
		if (str[1]) {
			r_cmd_alias_del (core->rcmd, str + 2);
		} else {
			r_cmd_alias_del (core->rcmd, NULL);
		}
		break;
	case '?':
		eprintf ("Usage: =$[-][remotecmd]  # remote command alias\n");
		eprintf (" =$dr   # makes 'dr' alias for =!dr\n");
		eprintf (" =$-dr  # unset 'dr' alias\n");
		break;
	case 0:
		r_core_cmd0 (core, "$");
		break;
	default:
		r_cmd_alias_set (core->rcmd, str, "", 1);
		break;
	}
}