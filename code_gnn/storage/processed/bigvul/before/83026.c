static int etm_addr_filters_validate(struct list_head *filters)
{
	bool range = false, address = false;
	int index = 0;
	struct perf_addr_filter *filter;

	list_for_each_entry(filter, filters, entry) {
		 
		if (++index > ETM_ADDR_CMP_MAX)
			return -EOPNOTSUPP;

		 
		if ((filter->range == 1) && address)
			return -EOPNOTSUPP;

		if ((filter->range == 0) && range)
			return -EOPNOTSUPP;

		 
		if (filter->range && filter->size == 0)
			return -EINVAL;

		 
		if (filter->range)
			range = true;
		else
			address = true;
	}

	return 0;
}
