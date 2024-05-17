static void __ftrace_hash_rec_update(struct ftrace_ops *ops,
				     int filter_hash,
				     bool inc)
{
	struct ftrace_hash *hash;
	struct ftrace_hash *other_hash;
	struct ftrace_page *pg;
	struct dyn_ftrace *rec;
	int count = 0;
	int all = 0;

	 
	if (!(ops->flags & FTRACE_OPS_FL_ENABLED))
		return;

	 
	if (filter_hash) {
		hash = ops->filter_hash;
		other_hash = ops->notrace_hash;
		if (ftrace_hash_empty(hash))
			all = 1;
	} else {
		inc = !inc;
		hash = ops->notrace_hash;
		other_hash = ops->filter_hash;
		 
		if (ftrace_hash_empty(hash))
			return;
	}

	do_for_each_ftrace_rec(pg, rec) {
		int in_other_hash = 0;
		int in_hash = 0;
		int match = 0;

		if (all) {
			 
			if (!other_hash || !ftrace_lookup_ip(other_hash, rec->ip))
				match = 1;
		} else {
			in_hash = !!ftrace_lookup_ip(hash, rec->ip);
			in_other_hash = !!ftrace_lookup_ip(other_hash, rec->ip);

			 
			if (filter_hash && in_hash && !in_other_hash)
				match = 1;
			else if (!filter_hash && in_hash &&
				 (in_other_hash || ftrace_hash_empty(other_hash)))
				match = 1;
		}
		if (!match)
			continue;

		if (inc) {
			rec->flags++;
			if (FTRACE_WARN_ON((rec->flags & ~FTRACE_FL_MASK) == FTRACE_REF_MAX))
				return;
			 
			if (ops->flags & FTRACE_OPS_FL_SAVE_REGS)
				rec->flags |= FTRACE_FL_REGS;
		} else {
			if (FTRACE_WARN_ON((rec->flags & ~FTRACE_FL_MASK) == 0))
				return;
			rec->flags--;
		}
		count++;
		 
		if (!all && count == hash->count)
			return;
	} while_for_each_ftrace_rec();
}