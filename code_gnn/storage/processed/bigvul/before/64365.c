R_API int r_core_cmd(RCore *core, const char *cstr, int log) {
	char *cmd, *ocmd, *ptr, *rcmd;
	int ret = false, i;

	r_th_lock_enter (core->lock);
	if (core->cmdfilter) {
		const char *invalid_chars = ";|>`@";
		for (i = 0; invalid_chars[i]; i++) {
			if (strchr (cstr, invalid_chars[i])) {
				ret = true;
				goto beach;
			}
		}
		if (strncmp (cstr, core->cmdfilter, strlen (core->cmdfilter))) {
			ret = true;
			goto beach;
		}
	}
	if (core->cmdremote) {
		if (*cstr != '=' && *cstr != 'q' && strncmp (cstr, "!=", 2)) {
			r_io_system (core->io, cstr);
			goto beach;  
		}
	}

	if (!cstr || *cstr == '|') {
		goto beach;  
	}
	if (!strncmp (cstr, "/*", 2)) {
		if (r_sandbox_enable (0)) {
			eprintf ("This command is disabled in sandbox mode\n");
			goto beach;  
		}
		core->incomment = true;
	} else if (!strncmp (cstr, "*/", 2)) {
		core->incomment = false;
		goto beach;  
	}
	if (core->incomment) {
		goto beach;  
	}
	if (log && (*cstr && (*cstr != '.' || !strncmp (cstr, ".(", 2)))) {
		free (core->lastcmd);
		core->lastcmd = strdup (cstr);
	}

	ocmd = cmd = malloc (strlen (cstr) + 4096);
	if (!ocmd) {
		goto beach;
	}
	r_str_cpy (cmd, cstr);
	if (log) {
		r_line_hist_add (cstr);
	}

	if (core->cmd_depth < 1) {
		eprintf ("r_core_cmd: That was too deep (%s)...\n", cmd);
		free (ocmd);
		free (core->oobi);
		core->oobi = NULL;
		core->oobi_len = 0;
		goto beach;
	}
	core->cmd_depth--;
	for (rcmd = cmd;;) {
		ptr = strchr (rcmd, '\n');
		if (ptr) {
			*ptr = '\0';
		}
		ret = r_core_cmd_subst (core, rcmd);
		if (ret == -1) {
			eprintf ("|ERROR| Invalid command '%s' (0x%02x)\n", rcmd, *rcmd);
			break;
		}
		if (!ptr) {
			break;
		}
		rcmd = ptr + 1;
	}
	r_th_lock_leave (core->lock);
	 
	if (core->anal->cmdtail) {
		char *res = core->anal->cmdtail;
		core->anal->cmdtail = NULL;
		r_core_cmd_lines (core, res);
		free (res);
	}
	core->cmd_depth++;
	free (ocmd);
	free (core->oobi);
	core->oobi = NULL;
	core->oobi_len = 0;
	return ret;
beach:
	r_th_lock_leave (core->lock);
	 
	if (core->anal->cmdtail) {
		char *res = core->anal->cmdtail;
		core->anal->cmdtail = NULL;
		r_core_cmd0 (core, res);
		free (res);
	}
	return ret;
}