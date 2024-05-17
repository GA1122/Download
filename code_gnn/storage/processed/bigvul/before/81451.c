int tracing_alloc_snapshot(void)
{
	struct trace_array *tr = &global_trace;
	int ret;

	ret = tracing_alloc_snapshot_instance(tr);
	WARN_ON(ret < 0);

	return ret;
}
