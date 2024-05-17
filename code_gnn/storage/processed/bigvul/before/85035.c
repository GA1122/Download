translate_table(struct net *net, struct xt_table_info *newinfo, void *entry0,
		const struct ip6t_replace *repl)
{
	struct xt_percpu_counter_alloc_state alloc_state = { 0 };
	struct ip6t_entry *iter;
	unsigned int *offsets;
	unsigned int i;
	int ret = 0;

	newinfo->size = repl->size;
	newinfo->number = repl->num_entries;

	 
	for (i = 0; i < NF_INET_NUMHOOKS; i++) {
		newinfo->hook_entry[i] = 0xFFFFFFFF;
		newinfo->underflow[i] = 0xFFFFFFFF;
	}

	offsets = xt_alloc_entry_offsets(newinfo->number);
	if (!offsets)
		return -ENOMEM;
	i = 0;
	 
	xt_entry_foreach(iter, entry0, newinfo->size) {
		ret = check_entry_size_and_hooks(iter, newinfo, entry0,
						 entry0 + repl->size,
						 repl->hook_entry,
						 repl->underflow,
						 repl->valid_hooks);
		if (ret != 0)
			goto out_free;
		if (i < repl->num_entries)
			offsets[i] = (void *)iter - entry0;
		++i;
		if (strcmp(ip6t_get_target(iter)->u.user.name,
		    XT_ERROR_TARGET) == 0)
			++newinfo->stacksize;
	}

	ret = -EINVAL;
	if (i != repl->num_entries)
		goto out_free;

	 
	for (i = 0; i < NF_INET_NUMHOOKS; i++) {
		 
		if (!(repl->valid_hooks & (1 << i)))
			continue;
		if (newinfo->hook_entry[i] == 0xFFFFFFFF)
			goto out_free;
		if (newinfo->underflow[i] == 0xFFFFFFFF)
			goto out_free;
	}

	if (!mark_source_chains(newinfo, repl->valid_hooks, entry0, offsets)) {
		ret = -ELOOP;
		goto out_free;
	}
	kvfree(offsets);

	 
	i = 0;
	xt_entry_foreach(iter, entry0, newinfo->size) {
		ret = find_check_entry(iter, net, repl->name, repl->size,
				       &alloc_state);
		if (ret != 0)
			break;
		++i;
	}

	if (ret != 0) {
		xt_entry_foreach(iter, entry0, newinfo->size) {
			if (i-- == 0)
				break;
			cleanup_entry(iter, net);
		}
		return ret;
	}

	return ret;
 out_free:
	kvfree(offsets);
	return ret;
}