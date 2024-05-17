static int do_replace_finish(struct net *net, struct ebt_replace *repl,
			      struct ebt_table_info *newinfo)
{
	int ret, i;
	struct ebt_counter *counterstmp = NULL;
	 
	struct ebt_table_info *table;
	struct ebt_table *t;

	 
	if (repl->num_counters) {
		unsigned long size = repl->num_counters * sizeof(*counterstmp);
		counterstmp = vmalloc(size);
		if (!counterstmp)
			return -ENOMEM;
	}

	newinfo->chainstack = NULL;
	ret = ebt_verify_pointers(repl, newinfo);
	if (ret != 0)
		goto free_counterstmp;

	ret = translate_table(net, repl->name, newinfo);

	if (ret != 0)
		goto free_counterstmp;

	t = find_table_lock(net, repl->name, &ret, &ebt_mutex);
	if (!t) {
		ret = -ENOENT;
		goto free_iterate;
	}

	 
	if (t->check && (ret = t->check(newinfo, repl->valid_hooks)))
		goto free_unlock;

	if (repl->num_counters && repl->num_counters != t->private->nentries) {
		BUGPRINT("Wrong nr. of counters requested\n");
		ret = -EINVAL;
		goto free_unlock;
	}

	 
	table = t->private;
	 
	if (!table->nentries && newinfo->nentries && !try_module_get(t->me)) {
		ret = -ENOENT;
		goto free_unlock;
	} else if (table->nentries && !newinfo->nentries)
		module_put(t->me);
	 
	write_lock_bh(&t->lock);
	if (repl->num_counters)
		get_counters(t->private->counters, counterstmp,
		   t->private->nentries);

	t->private = newinfo;
	write_unlock_bh(&t->lock);
	mutex_unlock(&ebt_mutex);
	 
	if (repl->num_counters &&
	   copy_to_user(repl->counters, counterstmp,
	   repl->num_counters * sizeof(struct ebt_counter))) {
		ret = -EFAULT;
	}
	else
		ret = 0;

	 
	EBT_ENTRY_ITERATE(table->entries, table->entries_size,
			  ebt_cleanup_entry, net, NULL);

	vfree(table->entries);
	if (table->chainstack) {
		for_each_possible_cpu(i)
			vfree(table->chainstack[i]);
		vfree(table->chainstack);
	}
	vfree(table);

	vfree(counterstmp);
	return ret;

free_unlock:
	mutex_unlock(&ebt_mutex);
free_iterate:
	EBT_ENTRY_ITERATE(newinfo->entries, newinfo->entries_size,
			  ebt_cleanup_entry, net, NULL);
free_counterstmp:
	vfree(counterstmp);
	 
	if (newinfo->chainstack) {
		for_each_possible_cpu(i)
			vfree(newinfo->chainstack[i]);
		vfree(newinfo->chainstack);
	}
	return ret;
}
