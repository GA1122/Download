int ebt_register_table(struct net *net, const struct ebt_table *input_table,
		       const struct nf_hook_ops *ops, struct ebt_table **res)
{
	struct ebt_table_info *newinfo;
	struct ebt_table *t, *table;
	struct ebt_replace_kernel *repl;
	int ret, i, countersize;
	void *p;

	if (input_table == NULL || (repl = input_table->table) == NULL ||
	    repl->entries == NULL || repl->entries_size == 0 ||
	    repl->counters != NULL || input_table->private != NULL) {
		BUGPRINT("Bad table data for ebt_register_table!!!\n");
		return -EINVAL;
	}

	 
	table = kmemdup(input_table, sizeof(struct ebt_table), GFP_KERNEL);
	if (!table) {
		ret = -ENOMEM;
		goto out;
	}

	countersize = COUNTER_OFFSET(repl->nentries) * nr_cpu_ids;
	newinfo = vmalloc(sizeof(*newinfo) + countersize);
	ret = -ENOMEM;
	if (!newinfo)
		goto free_table;

	p = vmalloc(repl->entries_size);
	if (!p)
		goto free_newinfo;

	memcpy(p, repl->entries, repl->entries_size);
	newinfo->entries = p;

	newinfo->entries_size = repl->entries_size;
	newinfo->nentries = repl->nentries;

	if (countersize)
		memset(newinfo->counters, 0, countersize);

	 
	newinfo->chainstack = NULL;
	for (i = 0; i < NF_BR_NUMHOOKS; i++) {
		if ((repl->valid_hooks & (1 << i)) == 0)
			newinfo->hook_entry[i] = NULL;
		else
			newinfo->hook_entry[i] = p +
				((char *)repl->hook_entry[i] - repl->entries);
	}
	ret = translate_table(net, repl->name, newinfo);
	if (ret != 0) {
		BUGPRINT("Translate_table failed\n");
		goto free_chainstack;
	}

	if (table->check && table->check(newinfo, table->valid_hooks)) {
		BUGPRINT("The table doesn't like its own initial data, lol\n");
		ret = -EINVAL;
		goto free_chainstack;
	}

	table->private = newinfo;
	rwlock_init(&table->lock);
	mutex_lock(&ebt_mutex);
	list_for_each_entry(t, &net->xt.tables[NFPROTO_BRIDGE], list) {
		if (strcmp(t->name, table->name) == 0) {
			ret = -EEXIST;
			BUGPRINT("Table name already exists\n");
			goto free_unlock;
		}
	}

	 
	if (newinfo->nentries && !try_module_get(table->me)) {
		ret = -ENOENT;
		goto free_unlock;
	}
	list_add(&table->list, &net->xt.tables[NFPROTO_BRIDGE]);
	mutex_unlock(&ebt_mutex);

	WRITE_ONCE(*res, table);

	if (!ops)
		return 0;

	ret = nf_register_net_hooks(net, ops, hweight32(table->valid_hooks));
	if (ret) {
		__ebt_unregister_table(net, table);
		*res = NULL;
	}

	return ret;
free_unlock:
	mutex_unlock(&ebt_mutex);
free_chainstack:
	if (newinfo->chainstack) {
		for_each_possible_cpu(i)
			vfree(newinfo->chainstack[i]);
		vfree(newinfo->chainstack);
	}
	vfree(newinfo->entries);
free_newinfo:
	vfree(newinfo);
free_table:
	kfree(table);
out:
	return ret;
}