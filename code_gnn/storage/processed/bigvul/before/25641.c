int __init init_hw_perf_events(void)
{
	pr_info("Performance events: ");

	if (!supported_pmu()) {
		pr_cont("No support for PMU type '%s'\n", sparc_pmu_type);
		return 0;
	}

	pr_cont("Supported PMU type is '%s'\n", sparc_pmu_type);

	perf_pmu_register(&pmu, "cpu", PERF_TYPE_RAW);
	register_die_notifier(&perf_event_nmi_notifier);

	return 0;
}
