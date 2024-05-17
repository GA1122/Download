static int compat_table_info(const struct xt_table_info *info,
			     struct xt_table_info *newinfo)
{
	struct ip6t_entry *iter;
	const void *loc_cpu_entry;
	int ret;

	if (!newinfo || !info)
		return -EINVAL;

	 
	memcpy(newinfo, info, offsetof(struct xt_table_info, entries));
	newinfo->initial_entries = 0;
	loc_cpu_entry = info->entries;
	xt_compat_init_offsets(AF_INET6, info->number);
	xt_entry_foreach(iter, loc_cpu_entry, info->size) {
		ret = compat_calc_entry(iter, info, loc_cpu_entry, newinfo);
		if (ret != 0)
			return ret;
	}
	return 0;
}