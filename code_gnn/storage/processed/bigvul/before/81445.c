__init static int tracer_alloc_buffers(void)
{
	int ring_buf_size;
	int ret = -ENOMEM;

	 
	BUILD_BUG_ON(TRACE_ITER_LAST_BIT > TRACE_FLAGS_MAX_SIZE);

	if (!alloc_cpumask_var(&tracing_buffer_mask, GFP_KERNEL))
		goto out;

	if (!alloc_cpumask_var(&global_trace.tracing_cpumask, GFP_KERNEL))
		goto out_free_buffer_mask;

	 
	if (__stop___trace_bprintk_fmt != __start___trace_bprintk_fmt)
		 
		trace_printk_init_buffers();

	 
	if (ring_buffer_expanded)
		ring_buf_size = trace_buf_size;
	else
		ring_buf_size = 1;

	cpumask_copy(tracing_buffer_mask, cpu_possible_mask);
	cpumask_copy(global_trace.tracing_cpumask, cpu_all_mask);

	raw_spin_lock_init(&global_trace.start_lock);

	 
	ret = cpuhp_setup_state_multi(CPUHP_TRACE_RB_PREPARE,
				      "trace/RB:preapre", trace_rb_cpu_prepare,
				      NULL);
	if (ret < 0)
		goto out_free_cpumask;
	 
	ret = -ENOMEM;
	temp_buffer = ring_buffer_alloc(PAGE_SIZE, RB_FL_OVERWRITE);
	if (!temp_buffer)
		goto out_rm_hp_state;

	if (trace_create_savedcmd() < 0)
		goto out_free_temp_buffer;

	 
	if (allocate_trace_buffers(&global_trace, ring_buf_size) < 0) {
		printk(KERN_ERR "tracer: failed to allocate ring buffer!\n");
		WARN_ON(1);
		goto out_free_savedcmd;
	}

	if (global_trace.buffer_disabled)
		tracing_off();

	if (trace_boot_clock) {
		ret = tracing_set_clock(&global_trace, trace_boot_clock);
		if (ret < 0)
			pr_warn("Trace clock %s not defined, going back to default\n",
				trace_boot_clock);
	}

	 
	global_trace.current_trace = &nop_trace;

	global_trace.max_lock = (arch_spinlock_t)__ARCH_SPIN_LOCK_UNLOCKED;

	ftrace_init_global_array_ops(&global_trace);

	init_trace_flags_index(&global_trace);

	register_tracer(&nop_trace);

	 
	init_function_trace();

	 
	tracing_disabled = 0;

	atomic_notifier_chain_register(&panic_notifier_list,
				       &trace_panic_notifier);

	register_die_notifier(&trace_die_notifier);

	global_trace.flags = TRACE_ARRAY_FL_GLOBAL;

	INIT_LIST_HEAD(&global_trace.systems);
	INIT_LIST_HEAD(&global_trace.events);
	INIT_LIST_HEAD(&global_trace.hist_vars);
	list_add(&global_trace.list, &ftrace_trace_arrays);

	apply_trace_boot_options();

	register_snapshot_cmd();

	return 0;

out_free_savedcmd:
	free_saved_cmdlines_buffer(savedcmd);
out_free_temp_buffer:
	ring_buffer_free(temp_buffer);
out_rm_hp_state:
	cpuhp_remove_multi_state(CPUHP_TRACE_RB_PREPARE);
out_free_cpumask:
	free_cpumask_var(global_trace.tracing_cpumask);
out_free_buffer_mask:
	free_cpumask_var(tracing_buffer_mask);
out:
	return ret;
}
