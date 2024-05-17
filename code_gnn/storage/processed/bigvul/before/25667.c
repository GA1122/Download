static void sparc_pmu_enable(struct pmu *pmu)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	u64 pcr;

	if (cpuc->enabled)
		return;

	cpuc->enabled = 1;
	barrier();

	pcr = cpuc->pcr;
	if (!cpuc->n_events) {
		pcr = 0;
	} else {
		pcr = maybe_change_configuration(cpuc, pcr);

		 
		cpuc->pcr = pcr | cpuc->event[0]->hw.config_base;
	}

	pcr_ops->write(cpuc->pcr);
}
