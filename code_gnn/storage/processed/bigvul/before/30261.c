register_ftrace_function_probe(char *glob, struct ftrace_probe_ops *ops,
			      void *data)
{
	struct ftrace_func_probe *entry;
	struct ftrace_page *pg;
	struct dyn_ftrace *rec;
	int type, len, not;
	unsigned long key;
	int count = 0;
	char *search;

	type = filter_parse_regex(glob, strlen(glob), &search, &not);
	len = strlen(search);

	 
	if (WARN_ON(not))
		return -EINVAL;

	mutex_lock(&ftrace_lock);

	if (unlikely(ftrace_disabled))
		goto out_unlock;

	do_for_each_ftrace_rec(pg, rec) {

		if (!ftrace_match_record(rec, NULL, search, len, type))
			continue;

		entry = kmalloc(sizeof(*entry), GFP_KERNEL);
		if (!entry) {
			 
			if (!count)
				count = -ENOMEM;
			goto out_unlock;
		}

		count++;

		entry->data = data;

		 
		if (ops->callback) {
			if (ops->callback(rec->ip, &entry->data) < 0) {
				 
				kfree(entry);
				continue;
			}
		}

		entry->ops = ops;
		entry->ip = rec->ip;

		key = hash_long(entry->ip, FTRACE_HASH_BITS);
		hlist_add_head_rcu(&entry->node, &ftrace_func_hash[key]);

	} while_for_each_ftrace_rec();
	__enable_ftrace_function_probe();

 out_unlock:
	mutex_unlock(&ftrace_lock);

	return count;
}