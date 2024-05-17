R_API int r_core_cmd_foreach(RCore *core, const char *cmd, char *each) {
	int i, j;
	char ch;
	char *word = NULL;
	char *str, *ostr;
	RListIter *iter;
	RFlagItem *flag;
	ut64 oseek, addr;

	for (; *cmd == ' '; cmd++);

	oseek = core->offset;
	ostr = str = strdup (each);
	r_cons_break_push (NULL, NULL);  
	switch (each[0]) {
	case '/':  
		{
		char *cmdhit = strdup (r_config_get (core->config, "cmd.hit"));
		r_config_set (core->config, "cmd.hit", cmd);
		r_core_cmd0 (core, each);
		r_config_set (core->config, "cmd.hit", cmdhit);
		free (cmdhit);
		}
		return 0;
	case '?':
		helpCmdForeach (core);
		break;
	case 'b':  
		{
			RListIter *iter;
			RAnalBlock *bb;
			RAnalFunction *fcn = r_anal_get_fcn_at (core->anal, core->offset, 0);
			int bs = core->blocksize;
			if (fcn) {
				r_list_sort (fcn->bbs, bb_cmp);
				r_list_foreach (fcn->bbs, iter, bb) {
					r_core_block_size (core, bb->size);
					r_core_seek (core, bb->addr, 1);
					r_core_cmd (core, cmd, 0);
					if (r_cons_is_breaked ()) {
						break;
					}
				}
			}
			free (ostr);
			r_core_block_size (core, bs);
			goto out_finish;
		}
		break;
	case 'i':  
		{
			RListIter *iter;
			RAnalBlock *bb;
			int i;
			RAnalFunction *fcn = r_anal_get_fcn_at (core->anal, core->offset, 0);
			if (fcn) {
				r_list_sort (fcn->bbs, bb_cmp);
				r_list_foreach (fcn->bbs, iter, bb) {
					for (i = 0; i < bb->op_pos_size; i++) {
						ut64 addr = bb->addr + bb->op_pos[i];
						r_core_seek (core, addr, 1);
						r_core_cmd (core, cmd, 0);
						if (r_cons_is_breaked ()) {
							break;
						}
					}
				}
			}
			free (ostr);
			goto out_finish;
		}
		break;
	case 'f':  
		if (each[1] == ':') {
			RAnalFunction *fcn;
			RListIter *iter;
			if (core->anal) {
				r_list_foreach (core->anal->fcns, iter, fcn) {
					if (each[2] && strstr (fcn->name, each + 2)) {
						r_core_seek (core, fcn->addr, 1);
						r_core_cmd (core, cmd, 0);
						if (r_cons_is_breaked ()) {
							break;
						}
					}
				}
			}
			free (ostr);
			goto out_finish;
		} else {
			RAnalFunction *fcn;
			RListIter *iter;
			if (core->anal) {
				RConsGrep grep = core->cons->grep;
				r_list_foreach (core->anal->fcns, iter, fcn) {
					char *buf;
					r_core_seek (core, fcn->addr, 1);
					r_cons_push ();
					r_core_cmd (core, cmd, 0);
					buf = (char *)r_cons_get_buffer ();
					if (buf) {
						buf = strdup (buf);
					}
					r_cons_pop ();
					r_cons_strcat (buf);
					free (buf);
					if (r_cons_is_breaked ()) {
						break;
					}
				}
				core->cons->grep = grep;
			}
			free (ostr);
			goto out_finish;
		}
		break;
	case 't':
		{
			RDebugPid *p;
			int pid = core->dbg->pid;
			if (core->dbg->h && core->dbg->h->pids) {
				RList *list = core->dbg->h->pids (core->dbg, R_MAX (0, pid));
				r_list_foreach (list, iter, p) {
					r_cons_printf ("# PID %d\n", p->pid);
					r_debug_select (core->dbg, p->pid, p->pid);
					r_core_cmd (core, cmd, 0);
					r_cons_newline ();
				}
				r_list_free (list);
			}
			r_debug_select (core->dbg, pid, pid);
			free (ostr);
			goto out_finish;
		}
		break;
	case 'c':  
		if (each[1] == ':') {
			char *arg = r_core_cmd_str (core, each + 2);
			if (arg) {
				foreachOffset (core, cmd, arg);
			}
		}
		break;
	case '=':
		foreachOffset (core, cmd, str + 1);
		break;
	case 'd':
		if (each[1] == 'b' && each[2] == 't') {
			ut64 oseek = core->offset;
			RDebugFrame *frame;
			RListIter *iter;
			RList *list;
			list = r_debug_frames (core->dbg, UT64_MAX);
			i = 0;
			r_list_foreach (list, iter, frame) {
				switch (each[3]) {
				case 'b':
					r_core_seek (core, frame->bp, 1);
					break;
				case 's':
					r_core_seek (core, frame->sp, 1);
					break;
				default:
				case 'a':
					r_core_seek (core, frame->addr, 1);
					break;
				}
				r_core_cmd (core, cmd, 0);
				r_cons_newline ();
				i++;
			}
			r_core_seek (core, oseek, 0);
			r_list_free (list);
		} else {
			eprintf("Invalid for-each statement. Use @@=dbt[abs]\n");
		}
		break;
	case 'k':
		 
		{
		char *out = sdb_querys (core->sdb, NULL, 0, str + ((str[1])? 2: 1));
		if (out) {
			each = out;
			do {
				while (*each == ' ') each++;
				if (!*each) {
					break;
				}
				str = strchr (each, ' ');
				if (str) {
					*str = '\0';
					addr = r_num_math (core->num, each);
					*str = ' ';
				} else {
					addr = r_num_math (core->num, each);
				}
				each = str + 1;
				r_core_seek (core, addr, 1);
				r_core_cmd (core, cmd, 0);
				r_cons_flush ();
			} while (str != NULL);
			free (out);
		}
		}
		break;
	case '.':
		if (each[1] == '(') {
			char cmd2[1024];
			i = 0;
			for (core->rcmd->macro.counter = 0; i < 999; core->rcmd->macro.counter++) {
				if (r_cons_is_breaked ()) {
					break;
				}
				r_cmd_macro_call (&core->rcmd->macro, each + 2);
				if (!core->rcmd->macro.brk_value) {
					break;
				}
				addr = core->rcmd->macro._brk_value;
				sprintf (cmd2, "%s @ 0x%08"PFMT64x"", cmd, addr);
				eprintf ("0x%08"PFMT64x" (%s)\n", addr, cmd2);
				r_core_seek (core, addr, 1);
				r_core_cmd (core, cmd2, 0);
				i++;
			}
		} else {
			char buf[1024];
			char cmd2[1024];
			FILE *fd = r_sandbox_fopen (each + 1, "r");
			if (fd) {
				core->rcmd->macro.counter=0;
				while (!feof (fd)) {
					buf[0] = '\0';
					if (!fgets (buf, sizeof (buf), fd)) {
						break;
					}
					addr = r_num_math (core->num, buf);
					eprintf ("0x%08"PFMT64x": %s\n", addr, cmd);
					sprintf (cmd2, "%s @ 0x%08"PFMT64x"", cmd, addr);
					r_core_seek (core, addr, 1);  
					r_core_cmd (core, cmd2, 0);
					core->rcmd->macro.counter++;
				}
				fclose (fd);
			} else {
				eprintf ("cannot open file '%s' to read offsets\n", each + 1);
			}
		}
		break;
	default:
		core->rcmd->macro.counter = 0;
		for (; *each == ' '; each++);
		i = 0;
		while (str[i]) {
			j = i;
			for (; str[j] && str[j] == ' '; j++);  
			for (i = j; str[i] && str[i] != ' '; i++);  
			ch = str[i];
			str[i] = '\0';
			word = strdup (str + j);
			if (!word) {
				break;
			}
			str[i] = ch;
			{
				int flagspace = core->flags->space_idx;
				 
				r_list_foreach (core->flags->flags, iter, flag) {
					if (r_cons_is_breaked ()) {
						break;
					}
					 
					if ((flagspace != -1) && (flag->space != flagspace)) {
						continue;
					}
					if (r_str_glob (flag->name, word)) {
						char *buf = NULL;
						const char *tmp = NULL;
						r_core_seek (core, flag->offset, 1);
						r_cons_push ();
						r_core_cmd (core, cmd, 0);
						tmp = r_cons_get_buffer ();
						buf = tmp? strdup (tmp): NULL;
						r_cons_pop ();
						r_cons_strcat (buf);
						free (buf);
					}
				}
				core->flags->space_idx = flagspace;
				core->rcmd->macro.counter++ ;
				free (word);
				word = NULL;
			}
		}
	}
	r_cons_break_pop ();
	core->offset = oseek;

	free (word);
	free (ostr);
	return true;
out_finish:
	r_cons_break_pop ();
	return false;
}