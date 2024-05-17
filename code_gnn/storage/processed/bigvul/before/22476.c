static u64 irq_time_cpu(int cpu)
{
	if (!sched_clock_irqtime)
		return 0;

	return per_cpu(cpu_softirq_time, cpu) + per_cpu(cpu_hardirq_time, cpu);
}
