static irqreturn_t armv8pmu_handle_irq(int irq_num, void *dev)
{
	u32 pmovsr;
	struct perf_sample_data data;
	struct pmu_hw_events *cpuc;
	struct pt_regs *regs;
	int idx;

	 
	pmovsr = armv8pmu_getreset_flags();

	 
	if (!armv8pmu_has_overflowed(pmovsr))
		return IRQ_NONE;

	 
	regs = get_irq_regs();

	cpuc = this_cpu_ptr(&cpu_hw_events);
	for (idx = 0; idx < cpu_pmu->num_events; ++idx) {
		struct perf_event *event = cpuc->events[idx];
		struct hw_perf_event *hwc;

		 
		if (!event)
			continue;

		 
		if (!armv8pmu_counter_has_overflowed(pmovsr, idx))
			continue;

		hwc = &event->hw;
		armpmu_event_update(event, hwc, idx);
		perf_sample_data_init(&data, 0, hwc->last_period);
		if (!armpmu_event_set_period(event, hwc, idx))
			continue;

		if (perf_event_overflow(event, &data, regs))
			cpu_pmu->disable(hwc, idx);
	}

	 
	irq_work_run();

	return IRQ_HANDLED;
}
