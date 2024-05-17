static int copy_entries_to_user(unsigned int total_size,
				const struct xt_table *table,
				void __user *userptr)
{
	unsigned int off, num;
	const struct arpt_entry *e;
	struct xt_counters *counters;
	struct xt_table_info *private = table->private;
	int ret = 0;
	void *loc_cpu_entry;

	counters = alloc_counters(table);
	if (IS_ERR(counters))
		return PTR_ERR(counters);

	loc_cpu_entry = private->entries;

	 
	 
	for (off = 0, num = 0; off < total_size; off += e->next_offset, num++){
		const struct xt_entry_target *t;

		e = loc_cpu_entry + off;
		if (copy_to_user(userptr + off, e, sizeof(*e))) {
			ret = -EFAULT;
			goto free_counters;
		}
		if (copy_to_user(userptr + off
				 + offsetof(struct arpt_entry, counters),
				 &counters[num],
				 sizeof(counters[num])) != 0) {
			ret = -EFAULT;
			goto free_counters;
		}

		t = arpt_get_target_c(e);
		if (xt_target_to_user(t, userptr + off + e->target_offset)) {
			ret = -EFAULT;
			goto free_counters;
		}
	}

 free_counters:
	vfree(counters);
	return ret;
}