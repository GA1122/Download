static void enable_trace_buffered_event(void *data)
{
	 
	smp_rmb();
	this_cpu_dec(trace_buffered_event_cnt);
}
