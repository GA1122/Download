void trace_printk_start_comm(void)
{
	 
	if (!buffers_allocated)
		return;
	tracing_start_cmdline_record();
}
