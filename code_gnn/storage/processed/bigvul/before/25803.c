static void intel_clovertown_quirks(void)
{
	 
	printk(KERN_WARNING "PEBS disabled due to CPU errata.\n");
	x86_pmu.pebs = 0;
	x86_pmu.pebs_constraints = NULL;
}
