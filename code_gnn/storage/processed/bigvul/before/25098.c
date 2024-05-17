static struct rtable *__rt_cache_get_next(struct seq_file *seq,
					  struct rtable *r)
{
	struct rt_cache_iter_state *st = seq->private;

	r = rcu_dereference_bh(r->dst.rt_next);
	while (!r) {
		rcu_read_unlock_bh();
		do {
			if (--st->bucket < 0)
				return NULL;
		} while (!rcu_dereference_raw(rt_hash_table[st->bucket].chain));
		rcu_read_lock_bh();
		r = rcu_dereference_bh(rt_hash_table[st->bucket].chain);
	}
	return r;
}
