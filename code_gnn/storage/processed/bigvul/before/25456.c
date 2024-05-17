static void fsl_emb_pmu_disable(struct pmu *pmu)
{
	struct cpu_hw_events *cpuhw;
	unsigned long flags;

	local_irq_save(flags);
	cpuhw = &__get_cpu_var(cpu_hw_events);

	if (!cpuhw->disabled) {
		cpuhw->disabled = 1;

		 
		if (!cpuhw->pmcs_enabled) {
			ppc_enable_pmcs();
			cpuhw->pmcs_enabled = 1;
		}

		if (atomic_read(&num_events)) {
			 

			mtpmr(PMRN_PMGC0, PMGC0_FAC);
			isync();
		}
	}
	local_irq_restore(flags);
}
