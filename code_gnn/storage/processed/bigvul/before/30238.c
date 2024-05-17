static int ftrace_update_code(struct module *mod)
{
	struct ftrace_page *pg;
	struct dyn_ftrace *p;
	cycle_t start, stop;
	unsigned long ref = 0;
	int i;

	 
	if (mod) {
		struct ftrace_ops *ops;

		for (ops = ftrace_ops_list;
		     ops != &ftrace_list_end; ops = ops->next) {
			if (ops->flags & FTRACE_OPS_FL_ENABLED &&
			    ops_traces_mod(ops))
				ref++;
		}
	}

	start = ftrace_now(raw_smp_processor_id());
	ftrace_update_cnt = 0;

	for (pg = ftrace_new_pgs; pg; pg = pg->next) {

		for (i = 0; i < pg->index; i++) {
			 
			if (unlikely(ftrace_disabled))
				return -1;

			p = &pg->records[i];
			p->flags = ref;

			 
			if (!ftrace_code_disable(mod, p))
				break;

			ftrace_update_cnt++;

			 
			if (ftrace_start_up && ref) {
				int failed = __ftrace_replace_code(p, 1);
				if (failed)
					ftrace_bug(failed, p->ip);
			}
		}
	}

	ftrace_new_pgs = NULL;

	stop = ftrace_now(raw_smp_processor_id());
	ftrace_update_time = stop - start;
	ftrace_update_tot_cnt += ftrace_update_cnt;

	return 0;
}
