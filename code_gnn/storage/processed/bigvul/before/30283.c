static void *t_start(struct seq_file *m, loff_t *pos)
{
	struct ftrace_iterator *iter = m->private;
	struct ftrace_ops *ops = iter->ops;
	void *p = NULL;
	loff_t l;

	mutex_lock(&ftrace_lock);

	if (unlikely(ftrace_disabled))
		return NULL;

	 
	if (*pos < iter->pos)
		reset_iter_read(iter);

	 
	if (iter->flags & FTRACE_ITER_FILTER &&
	    ftrace_hash_empty(ops->filter_hash)) {
		if (*pos > 0)
			return t_hash_start(m, pos);
		iter->flags |= FTRACE_ITER_PRINTALL;
		 
		iter->flags &= ~FTRACE_ITER_HASH;
		return iter;
	}

	if (iter->flags & FTRACE_ITER_HASH)
		return t_hash_start(m, pos);

	 
	iter->pg = ftrace_pages_start;
	iter->idx = 0;
	for (l = 0; l <= *pos; ) {
		p = t_next(m, p, &l);
		if (!p)
			break;
	}

	if (!p)
		return t_hash_start(m, pos);

	return iter;
}
