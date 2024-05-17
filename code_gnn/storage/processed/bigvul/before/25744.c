static inline __kprobes int notify_page_fault(struct pt_regs *regs)
{
	int ret = 0;

	 
	if (kprobes_built_in() && !user_mode(regs)) {
		preempt_disable();
		if (kprobe_running() && kprobe_fault_handler(regs, 0))
			ret = 1;
		preempt_enable();
	}
	return ret;
}
