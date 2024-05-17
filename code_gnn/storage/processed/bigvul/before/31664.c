static __init void intel_clovertown_quirk(void)
{
	 
	pr_warn("PEBS disabled due to CPU errata\n");
	x86_pmu.pebs = 0;
	x86_pmu.pebs_constraints = NULL;
}
