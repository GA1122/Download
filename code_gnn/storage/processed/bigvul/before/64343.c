static int cmd_alias(void *data, const char *input) {
	int i;
	char *def, *q, *desc, *buf;
	RCore *core = (RCore *)data;
	if (*input == '?') {
		const char* help_msg[] = {
			"Usage:", "$alias[=cmd] [args...]", "Alias commands",
			"$", "", "list all defined aliases",
			"$*", "", "same as above, but using r2 commands",
			"$", "dis='af;pdf'", "create command - analyze to show function",
			"$", "test=#!pipe node /tmp/test.js", "create command - rlangpipe script",
			"$", "dis=", "undefine alias",
			"$", "dis", "execute the previously defined alias",
			"$", "dis?", "show commands aliased by 'analyze'",
			NULL};
			r_core_cmd_help (core, help_msg);
		return 0;
	}
	i = strlen (input);
	buf = malloc (i + 2);
	if (!buf) {
		return 0;
	}
	*buf = '$';  
	memcpy (buf + 1, input, i + 1);
	q = strchr (buf, ' ');
	def = strchr (buf, '=');
	desc = strchr (buf, '?');

	 
	if ((def && q && (def < q)) || (def && !q)) {
		*def++ = 0;
		size_t len = strlen (def);
		 
		if ((def[0] == '\'') && (def[len - 1] == '\'')) {
			def[len - 1] = 0x00;
			def++;
		}
		if (!q || (q && q>def)) {
			if (*def) r_cmd_alias_set (core->rcmd, buf, def, 0);
			else r_cmd_alias_del (core->rcmd, buf);
		}
	 
	} else if (desc && !q) {
		char *v;
		*desc = 0;
		v = r_cmd_alias_get (core->rcmd, buf, 0);
		if (v) {
			r_cons_println (v);
			free (buf);
			return 1;
		} else {
			eprintf ("unknown key '%s'\n", buf);
		}

	 
	} else if (buf[1] == '*') {
		int i, count = 0;
		char **keys = r_cmd_alias_keys (core->rcmd, &count);
		for (i = 0; i < count; i++) {
			const char *v = r_cmd_alias_get (core->rcmd, keys[i], 0);
			r_cons_printf ("%s=%s\n", keys[i], v);
		}
	} else if (!buf[1]) {
		int i, count = 0;
		char **keys = r_cmd_alias_keys (core->rcmd, &count);
		for (i = 0; i < count; i++) {
			r_cons_println (keys[i]);
		}
	 
	} else {
		char *v;
		if (q) *q = 0;
		v = r_cmd_alias_get (core->rcmd, buf, 0);
		if (v) {
			if (q) {
				char *out, *args = q + 1;
				out = malloc (strlen (v) + strlen (args) + 2);
				if (out) {  
					strcpy (out, v);
					strcat (out, " ");
					strcat (out, args);
					r_core_cmd0 (core, out);
					free (out);
				} else eprintf ("cannot malloc\n");
			} else {
				r_core_cmd0 (core, v);
			}
		} else {
			eprintf ("unknown key '%s'\n", buf);
		}
	}
	free (buf);
	return 0;
}