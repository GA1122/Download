static void foreachOffset (RCore *core, const char *_cmd, const char *each) {
	char *cmd = strdup (_cmd);
	char *str = cmd;
	char *nextLine = NULL;
	ut64 addr;
	 
	while (each) {
		while (*each == ' ') {
			each++;
		}
		if (!*each) {
			break;
		}
		char *nl = strchr (each, '\n');
		if (nl) {
			*nl = 0;
			nextLine = nl + 1;
		} else {
			nextLine = NULL;
		}
		nl = strchr (each, '#');
		if (nl) {
			*nl = 0;
		}
		while (each && *each) {
			while (*each== ' ') each++;
			str = strchr (each, ' ');
			if (str) {
				*str = '\0';
				addr = r_num_math (core->num, each);
				*str = ' ';
				each = str + 1;
			} else {
				if (!*each) {
					break;
				}
				addr = r_num_math (core->num, each);
				each = NULL;
			}
			r_core_seek (core, addr, 1);
			r_core_cmd (core, cmd, 0);
			r_cons_flush ();
		}
		each = nextLine;
	}
	free (cmd);
}
