static struct inet6_ifaddr *if6_get_first(struct seq_file *seq, loff_t pos)
{
	struct inet6_ifaddr *ifa = NULL;
	struct if6_iter_state *state = seq->private;
	struct net *net = seq_file_net(seq);
	int p = 0;

	 
	if (pos == 0) {
		state->bucket = 0;
		state->offset = 0;
	}

	for (; state->bucket < IN6_ADDR_HSIZE; ++state->bucket) {
		hlist_for_each_entry_rcu_bh(ifa, &inet6_addr_lst[state->bucket],
					 addr_lst) {
			if (!net_eq(dev_net(ifa->idev->dev), net))
				continue;
			 
			if (p < state->offset) {
				p++;
				continue;
			}
			state->offset++;
			return ifa;
		}

		 
		state->offset = 0;
		p = 0;
	}
	return NULL;
}
