void kgdb_arch_late(void)
{
	int i, cpu;
	struct perf_event_attr attr;
	struct perf_event **pevent;

	 
	hw_breakpoint_init(&attr);
	attr.bp_addr = (unsigned long)kgdb_arch_init;
	attr.bp_len = HW_BREAKPOINT_LEN_1;
	attr.bp_type = HW_BREAKPOINT_W;
	attr.disabled = 1;
	for (i = 0; i < HBP_NUM; i++) {
		if (breakinfo[i].pev)
			continue;
		breakinfo[i].pev = register_wide_hw_breakpoint(&attr, NULL);
		if (IS_ERR((void * __force)breakinfo[i].pev)) {
			printk(KERN_ERR "kgdb: Could not allocate hw"
			       "breakpoints\nDisabling the kernel debugger\n");
			breakinfo[i].pev = NULL;
			kgdb_arch_exit();
			return;
		}
		for_each_online_cpu(cpu) {
			pevent = per_cpu_ptr(breakinfo[i].pev, cpu);
			pevent[0]->hw.sample_period = 1;
			pevent[0]->overflow_handler = kgdb_hw_overflow_handler;
			if (pevent[0]->destroy != NULL) {
				pevent[0]->destroy = NULL;
				release_bp_slot(*pevent);
			}
		}
	}
}