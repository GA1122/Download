static struct rtable *rt_cache_get_first(struct seq_file *seq)
{
	struct rt_cache_iter_state *st = seq->private;
	struct rtable *r = NULL;

	for (st->bucket = rt_hash_mask; st->bucket >= 0; --st->bucket) {
		if (!rcu_dereference_raw(rt_hash_table[st->bucket].chain))
			continue;
		rcu_read_lock_bh();
		r = rcu_dereference_bh(rt_hash_table[st->bucket].chain);
		while (r) {
			if (dev_net(r->dst.dev) == seq_file_net(seq) &&
			    r->rt_genid == st->genid)
				return r;
			r = rcu_dereference_bh(r->dst.rt_next);
		}
		rcu_read_unlock_bh();
	}
	return r;
}
