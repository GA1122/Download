static __init int p4_pmu_init(void)
{
	unsigned int low, high;

	 
	BUILD_BUG_ON(ARCH_P4_MAX_CCCR > X86_PMC_MAX_GENERIC);

	rdmsr(MSR_IA32_MISC_ENABLE, low, high);
	if (!(low & (1 << 7))) {
		pr_cont("unsupported Netburst CPU model %d ",
			boot_cpu_data.x86_model);
		return -ENODEV;
	}

	memcpy(hw_cache_event_ids, p4_hw_cache_event_ids,
		sizeof(hw_cache_event_ids));

	pr_cont("Netburst events, ");

	x86_pmu = p4_pmu;

	return 0;
}