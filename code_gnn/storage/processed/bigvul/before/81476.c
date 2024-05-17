int tracing_is_enabled(void)
{
	 
	smp_rmb();
	return !global_trace.buffer_disabled;
}
