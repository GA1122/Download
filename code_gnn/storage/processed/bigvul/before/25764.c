void perf_events_lapic_init(void)
{
	if (!x86_pmu.apic || !x86_pmu_initialized())
		return;

	 
	apic_write(APIC_LVTPC, APIC_DM_NMI);
}