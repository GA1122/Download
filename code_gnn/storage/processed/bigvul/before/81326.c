static int run_tracer_selftest(struct tracer *type)
{
	struct trace_array *tr = &global_trace;
	struct tracer *saved_tracer = tr->current_trace;
	int ret;

	if (!type->selftest || tracing_selftest_disabled)
		return 0;

	 
	if (!selftests_can_run)
		return save_selftest(type);

	 
	tracing_reset_online_cpus(&tr->trace_buffer);

	tr->current_trace = type;

#ifdef CONFIG_TRACER_MAX_TRACE
	if (type->use_max_tr) {
		 
		if (ring_buffer_expanded)
			ring_buffer_resize(tr->max_buffer.buffer, trace_buf_size,
					   RING_BUFFER_ALL_CPUS);
		tr->allocated_snapshot = true;
	}
#endif

	 
	pr_info("Testing tracer %s: ", type->name);
	ret = type->selftest(type, tr);
	 
	tr->current_trace = saved_tracer;
	if (ret) {
		printk(KERN_CONT "FAILED!\n");
		 
		WARN_ON(1);
		return -1;
	}
	 
	tracing_reset_online_cpus(&tr->trace_buffer);

#ifdef CONFIG_TRACER_MAX_TRACE
	if (type->use_max_tr) {
		tr->allocated_snapshot = false;

		 
		if (ring_buffer_expanded)
			ring_buffer_resize(tr->max_buffer.buffer, 1,
					   RING_BUFFER_ALL_CPUS);
	}
#endif

	printk(KERN_CONT "PASSED\n");
	return 0;
}
