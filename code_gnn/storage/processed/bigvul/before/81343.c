static int __init set_cmdline_ftrace(char *str)
{
	strlcpy(bootup_tracer_buf, str, MAX_TRACER_SIZE);
	default_bootup_tracer = bootup_tracer_buf;
	 
	ring_buffer_expanded = true;
	return 1;
}
