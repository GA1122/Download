void perf_event_print_debug(void)
{
	unsigned long flags;
	u64 pcr, pic;
	int cpu;

	if (!sparc_pmu)
		return;

	local_irq_save(flags);

	cpu = smp_processor_id();

	pcr = pcr_ops->read();
	read_pic(pic);

	pr_info("\n");
	pr_info("CPU#%d: PCR[%016llx] PIC[%016llx]\n",
		cpu, pcr, pic);

	local_irq_restore(flags);
}
