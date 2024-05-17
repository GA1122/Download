void account_system_vtime(struct task_struct *curr)
{
	unsigned long flags;
	int cpu;
	u64 now, delta;

	if (!sched_clock_irqtime)
		return;

	local_irq_save(flags);

	cpu = smp_processor_id();
	now = sched_clock_cpu(cpu);
	delta = now - per_cpu(irq_start_time, cpu);
	per_cpu(irq_start_time, cpu) = now;
	 
	if (hardirq_count())
		per_cpu(cpu_hardirq_time, cpu) += delta;
	else if (in_serving_softirq() && !(curr->flags & PF_KSOFTIRQD))
		per_cpu(cpu_softirq_time, cpu) += delta;

	local_irq_restore(flags);
}
