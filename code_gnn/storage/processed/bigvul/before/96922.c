void tracing_snapshot_cond(struct trace_array *tr, void *cond_data)
{
	WARN_ONCE(1, "Snapshot feature not enabled, but internal conditional snapshot used");
}
