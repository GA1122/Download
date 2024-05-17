static int cmd_kuery(void *data, const char *input) {
	char buf[1024], *out;
	RCore *core = (RCore*)data;
	const char *sp, *p = "[sdb]> ";
	const int buflen = sizeof (buf) - 1;
	Sdb *s = core->sdb;

	switch (input[0]) {
	case ' ':
		out = sdb_querys (s, NULL, 0, input + 1);
		if (out) {
			r_cons_println (out);
		}
		free (out);
		break;
	case '\0':
		sdb_foreach (s, callback_foreach_kv, NULL);
		break;
	case 's':
		if (core->http_up) {
			return false;
		}
		if (!r_config_get_i (core->config, "scr.interactive")) {
			return false;
		}
		if (input[1] == ' ') {
			char *n, *o, *p = strdup (input + 2);
			for (n = o = p; n; o = n) {
				n = strchr (o, '/');  
				if (n) *n++ = 0;
				s = sdb_ns (s, o, 1);
			}
			free (p);
		}
		if (!s) s = core->sdb;
		for (;;) {
			r_line_set_prompt (p);
			if (r_cons_fgets (buf, buflen, 0, NULL) < 1)
				break;
			if (!*buf) break;
			out = sdb_querys (s, NULL, 0, buf);
			if (out) {
				r_cons_println (out);
			}
		}
		break;
	case 'o':
		if (r_sandbox_enable (0)) {
			eprintf ("This command is disabled in sandbox mode\n");
			return 0;
		}
		if (input[1] == ' ') {
			char *fn = strdup (input + 2);
			if (!fn) {
				eprintf("Unable to allocate memory\n");
				return 0;
			}
			char *ns = strchr (fn, ' ');
			if (ns) {
				Sdb *db;
				*ns++ = 0;
				if (r_file_exists (fn)) {
					db = sdb_ns_path (core->sdb, ns, 1);
					if (db) {
						Sdb *newdb = sdb_new (NULL, fn, 0);
						if (newdb) {
							sdb_drain  (db, newdb);
						} else {
							eprintf ("Cannot open sdb '%s'\n", fn);
						}
					} else eprintf ("Cannot find sdb '%s'\n", ns);
				} else eprintf ("Cannot open file\n");
			} else eprintf ("Missing sdb namespace\n");
			free (fn);
		} else {
			eprintf ("Usage: ko [file] [namespace]\n");
		}
		break;
	case 'd':
		if (r_sandbox_enable (0)) {
			eprintf ("This command is disabled in sandbox mode\n");
			return 0;
		}
		if (input[1] == ' ') {
			char *fn = strdup (input + 2);
			char *ns = strchr (fn, ' ');
			if (ns) {
				*ns++ = 0;
				Sdb *db = sdb_ns_path (core->sdb, ns, 0);
				if (db) {
					sdb_file (db, fn);
					sdb_sync (db);
				} else eprintf ("Cannot find sdb '%s'\n", ns);
			} else eprintf ("Missing sdb namespace\n");
			free (fn);
		} else {
			eprintf ("Usage: kd [file] [namespace]\n");
		}
		break;
	case '?': {
			const char* help_msg[] = {
			"Usage:", "k[s] [key[=value]]", "Sdb Query",
			"k", " foo=bar", "set value",
			"k", " foo", "show value",
			"k", "", "list keys",
			"ko", " [file.sdb] [ns]", "open file into namespace",
			"kd", " [file.sdb] [ns]", "dump namespace to disk",
			"ks", " [ns]", "enter the sdb query shell",
			"k", " anal/meta/*", "ist kv from anal > meta namespaces",
			"k", " anal/**", "list namespaces under anal",
			"k", " anal/meta/meta.0x80404", "get value for meta.0x80404 key",
			NULL,
			};
			r_core_cmd_help (core, help_msg);
		}
		break;
	}

	if (input[0] == '\0') {
		 
		return 0;
	}

	sp = strchr (input + 1, ' ');
	if (sp) {
		char *inp = strdup (input);
		inp [(size_t)(sp - input)] = 0;
		s = sdb_ns (core->sdb, inp + 1, 1);
		out = sdb_querys (s, NULL, 0, sp + 1);
		if (out) {
			r_cons_println (out);
			free (out);
		}
		free (inp);
		return 0;
	}
	return 0;
}