static int cmd_rap(void *data, const char *input) {
	RCore *core = (RCore *)data;
	switch (*input) {
	case '$': aliascmd (core, input + 1); break;
	case '\0': r_core_rtr_list (core); break;
	case 'h':
		r_core_rtr_http (core, getArg (input[1], 'h'), input + 1);
		break;
	case 'H':
		while (input[1] == ' ') {
			input++;
		}
		r_core_rtr_http (core, getArg (input[1], 'H'), input + 1);
		break;
	case 'g':
		r_core_rtr_gdb (core, getArg (input[1], 'g'), input + 1);
		break;
	case '?': r_core_rtr_help (core); break;
	case '+': r_core_rtr_add (core, input + 1); break;
	case '-': r_core_rtr_remove (core, input + 1); break;
	case '=': r_core_rtr_session (core, input + 1); break;
	case '<': r_core_rtr_pushout (core, input + 1); break;
	case '!':
		if (input[1] == '=') {
			core->cmdremote = input[2]? 1: 0;
			r_cons_println (r_str_bool (core->cmdremote));
		} else {
			r_io_system (core->io, input + 1);
		}
		break;
	default:
		r_core_rtr_cmd (core, input);
		break;
	}
	return 0;
}