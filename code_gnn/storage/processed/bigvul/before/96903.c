void ftrace_dump(enum ftrace_dump_mode oops_dump_mode)
{
	 
	static struct trace_iterator iter;
	static atomic_t dump_running;
	struct trace_array *tr = &global_trace;
	unsigned int old_userobj;
	unsigned long flags;
	int cnt = 0, cpu;

	 
	if (atomic_inc_return(&dump_running) != 1) {
		atomic_dec(&dump_running);
		return;
	}

	 
	tracing_off();

	local_irq_save(flags);
	printk_nmi_direct_enter();

	 
	trace_init_global_iter(&iter);

	for_each_tracing_cpu(cpu) {
		atomic_inc(&per_cpu_ptr(iter.trace_buffer->data, cpu)->disabled);
	}

	old_userobj = tr->trace_flags & TRACE_ITER_SYM_USEROBJ;

	 
	tr->trace_flags &= ~TRACE_ITER_SYM_USEROBJ;

	switch (oops_dump_mode) {
	case DUMP_ALL:
		iter.cpu_file = RING_BUFFER_ALL_CPUS;
		break;
	case DUMP_ORIG:
		iter.cpu_file = raw_smp_processor_id();
		break;
	case DUMP_NONE:
		goto out_enable;
	default:
		printk(KERN_TRACE "Bad dumping mode, switching to all CPUs dump\n");
		iter.cpu_file = RING_BUFFER_ALL_CPUS;
	}

	printk(KERN_TRACE "Dumping ftrace buffer:\n");

	 
	if (ftrace_is_dead()) {
		printk("# WARNING: FUNCTION TRACING IS CORRUPTED\n");
		printk("#          MAY BE MISSING FUNCTION EVENTS\n");
	}

	 

	while (!trace_empty(&iter)) {

		if (!cnt)
			printk(KERN_TRACE "---------------------------------\n");

		cnt++;

		 
		memset(&iter.seq, 0,
		       sizeof(struct trace_iterator) -
		       offsetof(struct trace_iterator, seq));
		iter.iter_flags |= TRACE_FILE_LAT_FMT;
		iter.pos = -1;

		if (trace_find_next_entry_inc(&iter) != NULL) {
			int ret;

			ret = print_trace_line(&iter);
			if (ret != TRACE_TYPE_NO_CONSUME)
				trace_consume(&iter);
		}
		touch_nmi_watchdog();

		trace_printk_seq(&iter.seq);
	}

	if (!cnt)
		printk(KERN_TRACE "   (ftrace buffer empty)\n");
	else
		printk(KERN_TRACE "---------------------------------\n");

 out_enable:
	tr->trace_flags |= old_userobj;

	for_each_tracing_cpu(cpu) {
		atomic_dec(&per_cpu_ptr(iter.trace_buffer->data, cpu)->disabled);
	}
	atomic_dec(&dump_running);
	printk_nmi_direct_exit();
	local_irq_restore(flags);
}