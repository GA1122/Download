xt_replace_table(struct xt_table *table,
	      unsigned int num_counters,
	      struct xt_table_info *newinfo,
	      int *error)
{
	struct xt_table_info *private;
	int ret;

	ret = xt_jumpstack_alloc(newinfo);
	if (ret < 0) {
		*error = ret;
		return NULL;
	}

	 
	local_bh_disable();
	private = table->private;

	 
	if (num_counters != private->number) {
		pr_debug("num_counters != table->private->number (%u/%u)\n",
			 num_counters, private->number);
		local_bh_enable();
		*error = -EAGAIN;
		return NULL;
	}

	newinfo->initial_entries = private->initial_entries;
	 
	smp_wmb();
	table->private = newinfo;

	 
	local_bh_enable();

#ifdef CONFIG_AUDIT
	if (audit_enabled) {
		struct audit_buffer *ab;

		ab = audit_log_start(current->audit_context, GFP_KERNEL,
				     AUDIT_NETFILTER_CFG);
		if (ab) {
			audit_log_format(ab, "table=%s family=%u entries=%u",
					 table->name, table->af,
					 private->number);
			audit_log_end(ab);
		}
	}
#endif

	return private;
}