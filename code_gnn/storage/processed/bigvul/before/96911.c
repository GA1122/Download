static int trace_set_options(struct trace_array *tr, char *option)
{
	char *cmp;
	int neg = 0;
	int ret;
	size_t orig_len = strlen(option);
	int len;

	cmp = strstrip(option);

	len = str_has_prefix(cmp, "no");
	if (len)
		neg = 1;

	cmp += len;

	mutex_lock(&trace_types_lock);

	ret = match_string(trace_options, -1, cmp);
	 
	if (ret < 0)
		ret = set_tracer_option(tr, cmp, neg);
	else
		ret = set_tracer_flag(tr, 1 << ret, !neg);

	mutex_unlock(&trace_types_lock);

	 
	if (orig_len > strlen(option))
		option[strlen(option)] = ' ';

	return ret;
}