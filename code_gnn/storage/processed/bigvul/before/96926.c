void tracing_snapshot_instance(struct trace_array *tr)
{
	tracing_snapshot_instance_cond(tr, NULL);
}
