static int cmd_system(void *data, const char *input) {
	RCore *core = (RCore*)data;
	ut64 n;
	int ret = 0;
	switch (*input) {
	case '=':
		if (input[1] == '?') {
			r_cons_printf ("Usage: !=[!]  - enable/disable remote commands\n");
		} else {
			if (!r_sandbox_enable (0)) {
				core->cmdremote = input[1]? 1: 0;
				r_cons_println (r_str_bool (core->cmdremote));
			}
		}
		break;
	case '!':
		if (r_sandbox_enable (0)) {
			eprintf ("This command is disabled in sandbox mode\n");
			return 0;
		}
		if (input[1]) {
			int olen;
			char *out = NULL;
			char *cmd = r_core_sysenv_begin (core, input);
			if (cmd) {
				ret = r_sys_cmd_str_full (cmd + 1, NULL, &out, &olen, NULL);
				r_core_sysenv_end (core, input);
				r_cons_memcat (out, olen);
				free (out);
				free (cmd);
			}  
		} else {
			eprintf ("History saved to "R2_HOMEDIR"/history\n");
			r_line_hist_save (R2_HOMEDIR"/history");
		}
		break;
	case '\0':
		r_line_hist_list ();
		break;
	case '?':
		r_core_sysenv_help (core);
		break;
	default:
		n = atoi (input);
		if (*input == '0' || n > 0) {
			const char *cmd = r_line_hist_get (n);
			if (cmd) r_core_cmd0 (core, cmd);
		} else {
			char *cmd = r_core_sysenv_begin (core, input);
			if (cmd) {
				ret = r_sys_cmd (cmd);
				r_core_sysenv_end (core, input);
				free (cmd);
			} else eprintf ("Error setting up system environment\n");
		}
		break;
	}
	return ret;
}