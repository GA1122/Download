perf_event_set_addr_filter(struct perf_event *event, char *filter_str)
{
	LIST_HEAD(filters);
	int ret;

	 
	lockdep_assert_held(&event->ctx->mutex);

	if (WARN_ON_ONCE(event->parent))
		return -EINVAL;

	 
	if (!event->ctx->task)
		return -EOPNOTSUPP;

	ret = perf_event_parse_addr_filter(event, filter_str, &filters);
	if (ret)
		return ret;

	ret = event->pmu->addr_filters_validate(&filters);
	if (ret) {
		free_filters_list(&filters);
		return ret;
	}

	 
	perf_addr_filters_splice(event, &filters);

	 
	perf_event_for_each_child(event, perf_event_addr_filters_apply);

	return ret;
}
