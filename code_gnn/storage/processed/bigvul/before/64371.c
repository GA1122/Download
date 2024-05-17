R_API int r_core_cmd_foreach3(RCore *core, const char *cmd, char *each) {
	RDebug *dbg = core->dbg;
	RList *list, *head;
	RListIter *iter;
	RFlagItem *flg;
	int i;

	switch (each[0]) {
	case '=':
		{
		char *arg;
		for (arg = each + 1; ; ) {
			char *next = strchr (arg, ' ');
			if (next) {
				*next = 0;
			}
			if (arg && *arg) {
				r_core_cmdf (core, "%s %s", cmd, arg);
			}
			if (!next) {
				break;
			}
			arg = next + 1;
		}
		}
		break;
	case '?':
		r_cons_printf ("Usage: @@@ [type]     # types:\n"
			" symbols\n"
			" imports\n"
			" regs\n"
			" threads\n"
			" comments\n"
			" functions\n"
			" flags\n");
		break;
	case 'c':
		switch (each[1]) {
		case 'a':  
			break;
		default:
			r_meta_list_cb (core->anal, R_META_TYPE_COMMENT, 0, foreach_comment, (void*)cmd, UT64_MAX);
			break;
		}
		break;
	case 't':
		if (dbg && dbg->h && dbg->h->threads) {
			int origpid = dbg->pid;
			RDebugPid *p;
			list = dbg->h->threads (dbg, dbg->pid);
			if (!list)
				return false;
			r_list_foreach (list, iter, p) {
				r_core_cmdf (core, "dp %d", p->pid);
				r_cons_printf ("PID %d\n", p->pid);
				r_core_cmd0 (core, cmd);
			}
			r_core_cmdf (core, "dp %d", origpid);
			r_list_free (list);
		}
		break;
	case 'r':
		{
			ut64 offorig = core->offset;
			for (i = 0; i < 128; i++) {
				RRegItem *item;
				ut64 value;
				head = r_reg_get_list (dbg->reg, i);
				if (!head) {
					continue;
				}
				r_list_foreach (head, iter, item) {
					if (item->size != core->anal->bits) {
						continue;
					}
					value = r_reg_get_value (dbg->reg, item);
					r_core_seek (core, value, 1);
					r_cons_printf ("%s: ", item->name);
					r_core_cmd0 (core, cmd);
				}
			}
			r_core_seek (core, offorig, 1);
		}
		break;
	case 'i':
		{
			RBinImport *imp;
			ut64 offorig = core->offset;
			list = r_bin_get_imports (core->bin);
			r_list_foreach (list, iter, imp) {
				char *impflag = r_str_newf ("sym.imp.%s", imp->name);
				ut64 addr = r_num_math (core->num, impflag);
				if (addr && addr != UT64_MAX) {
					r_core_seek (core, addr, 1);
					r_core_cmd0 (core, cmd);
				}
			}
			r_core_seek (core, offorig, 1);
		}
		break;
	case 's':
		{
			RBinSymbol *sym;
			ut64 offorig = core->offset;
			list = r_bin_get_symbols (core->bin);
			r_list_foreach (list, iter, sym) {
				r_core_seek (core, sym->vaddr, 1);
				r_core_cmd0 (core, cmd);
			}
			r_core_seek (core, offorig, 1);
		}
		break;
	case 'f':
		switch (each[1]) {
		case 'l':  
			r_list_foreach (core->flags->flags, iter, flg) {
				r_core_seek (core, flg->offset, 1);
				r_core_cmd0 (core, cmd);
			}
			break;
		case 'u':  
			{
				ut64 offorig = core->offset;
				RAnalFunction *fcn;
				list = core->anal->fcns;
				r_list_foreach (list, iter, fcn) {
					r_cons_printf ("[0x%08"PFMT64x"  %s\n", fcn->addr, fcn->name);
					r_core_seek (core, fcn->addr, 1);
					r_core_cmd0 (core, cmd);
				}
				r_core_seek (core, offorig, 1);
			}
			break;
		}
		break;
	}
	return 0;
}