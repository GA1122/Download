perf_event_nmi_handler(struct notifier_block *self,
			 unsigned long cmd, void *__args)
{
	struct die_args *args = __args;
	unsigned int this_nmi;
	int handled;

	if (!atomic_read(&active_events))
		return NOTIFY_DONE;

	switch (cmd) {
	case DIE_NMI:
		break;
	case DIE_NMIUNKNOWN:
		this_nmi = percpu_read(irq_stat.__nmi_count);
		if (this_nmi != __this_cpu_read(pmu_nmi.marked))
			 
			return NOTIFY_DONE;
		 
		return NOTIFY_STOP;
	default:
		return NOTIFY_DONE;
	}

	handled = x86_pmu.handle_irq(args->regs);
	if (!handled)
		return NOTIFY_DONE;

	this_nmi = percpu_read(irq_stat.__nmi_count);
	if ((handled > 1) ||
		 
	    ((__this_cpu_read(pmu_nmi.marked) == this_nmi) &&
	     (__this_cpu_read(pmu_nmi.handled) > 1))) {
		 
		__this_cpu_write(pmu_nmi.marked, this_nmi + 1);
		__this_cpu_write(pmu_nmi.handled, handled);
	}

	return NOTIFY_STOP;
}
