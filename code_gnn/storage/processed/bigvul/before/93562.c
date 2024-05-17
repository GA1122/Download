static void *ipmr_mfc_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct mfc6_cache *mfc = v;
	struct ipmr_mfc_iter *it = seq->private;
	struct net *net = seq_file_net(seq);
	struct mr6_table *mrt = it->mrt;

	++*pos;

	if (v == SEQ_START_TOKEN)
		return ipmr_mfc_seq_idx(net, seq->private, 0);

	if (mfc->list.next != it->cache)
		return list_entry(mfc->list.next, struct mfc6_cache, list);

	if (it->cache == &mrt->mfc6_unres_queue)
		goto end_of_list;

	BUG_ON(it->cache != &mrt->mfc6_cache_array[it->ct]);

	while (++it->ct < MFC6_LINES) {
		it->cache = &mrt->mfc6_cache_array[it->ct];
		if (list_empty(it->cache))
			continue;
		return list_first_entry(it->cache, struct mfc6_cache, list);
	}

	 
	read_unlock(&mrt_lock);
	it->cache = &mrt->mfc6_unres_queue;
	it->ct = 0;

	spin_lock_bh(&mfc_unres_lock);
	if (!list_empty(it->cache))
		return list_first_entry(it->cache, struct mfc6_cache, list);

 end_of_list:
	spin_unlock_bh(&mfc_unres_lock);
	it->cache = NULL;

	return NULL;
}