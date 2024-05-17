int register_power_pmu(struct power_pmu *pmu)
{
	if (ppmu)
		return -EBUSY;		 

	ppmu = pmu;
	pr_info("%s performance monitor hardware support registered\n",
		pmu->name);

#ifdef MSR_HV
	 
	if (mfmsr() & MSR_HV)
		freeze_events_kernel = MMCR0_FCHV;
#endif  

	perf_pmu_register(&power_pmu, "cpu", PERF_TYPE_RAW);
	perf_cpu_notifier(power_pmu_notifier);

	return 0;
}
