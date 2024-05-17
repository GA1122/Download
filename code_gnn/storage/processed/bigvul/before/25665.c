static void sparc_pmu_disable(struct pmu *pmu)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	u64 val;

	if (!cpuc->enabled)
		return;

	cpuc->enabled = 0;
	cpuc->n_added = 0;

	val = cpuc->pcr;
	val &= ~(PCR_UTRACE | PCR_STRACE |
		 sparc_pmu->hv_bit | sparc_pmu->irq_bit);
	cpuc->pcr = val;

	pcr_ops->write(cpuc->pcr);
}
