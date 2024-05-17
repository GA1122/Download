int __init init_hw_perf_events(void)
{
	pr_info("Performance events: ");

	if (!supported_cpu()) {
		pr_cont("No support for your CPU.\n");
		return 0;
	}

	pr_cont("Supported CPU type!\n");

	 

	perf_irq = alpha_perf_event_irq_handler;

	 
	alpha_pmu = &ev67_pmu;

	perf_pmu_register(&pmu, "cpu", PERF_TYPE_RAW);

	return 0;
}
