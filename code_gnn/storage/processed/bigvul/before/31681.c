static void intel_pmu_flush_branch_stack(void)
{
	 
	if (x86_pmu.lbr_nr)
		intel_pmu_lbr_reset();
}
