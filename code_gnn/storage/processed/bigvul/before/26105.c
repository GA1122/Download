static int perf_event_set_filter(struct perf_event *event, void __user *arg)
{
	char *filter_str;
	int ret;

	if (event->attr.type != PERF_TYPE_TRACEPOINT)
		return -EINVAL;

	filter_str = strndup_user(arg, PAGE_SIZE);
	if (IS_ERR(filter_str))
		return PTR_ERR(filter_str);

	ret = ftrace_profile_set_filter(event, event->attr.config, filter_str);

	kfree(filter_str);
	return ret;
}
