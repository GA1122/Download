static int cmd_head (void *data, const char *_input) {  
	RCore *core = (RCore *)data;
	int lines = 5;
	char *input = strdup (_input);
	char *arg = strchr (input, ' ');
	char *tmp, *count;
	if (arg) {
		arg = (char *)r_str_trim_ro (arg + 1); 	 
		count = strchr (arg, ' ');
		if (count) {
			*count = 0;	 
			tmp = (char *)r_str_trim_ro (count + 1);
			lines = atoi (arg);
			arg = tmp;
		}
	}
	switch (*input) {
	case '?':  
		eprintf ("Usage: head [file] # to list first n lines in file\n");
		break;
	default:  
		if (!arg) {
			arg = "";
		}
		if (r_fs_check (core->fs, arg)) {
			r_core_cmdf (core, "md %s", arg);
		} else {
			char *res = r_syscmd_head (arg, lines);
			if (res) {
				r_cons_print (res);
				free (res);
			}
		}
		break;
	}
	free (input);
	return 0;
}