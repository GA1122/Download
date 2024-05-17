 xscale2pmu_handle_irq(int irq_num, void *dev)
 {
 	unsigned long pmnc, of_flags;
 	struct perf_sample_data data;
 	struct cpu_hw_events *cpuc;
 	struct pt_regs *regs;
 	int idx;
 
 	 
 	pmnc = xscale2pmu_read_pmnc();
 	xscale2pmu_write_pmnc(pmnc & ~XSCALE_PMU_ENABLE);
 
 	 
 	of_flags = xscale2pmu_read_overflow_flags();
 	if (!(of_flags & XSCALE2_OVERFLOWED_MASK))
 		return IRQ_NONE;
 
 	 
 	xscale2pmu_write_overflow_flags(of_flags);
 
 	regs = get_irq_regs();
 
 	perf_sample_data_init(&data, 0);
 
 	cpuc = &__get_cpu_var(cpu_hw_events);
 	for (idx = 0; idx <= armpmu->num_events; ++idx) {
 		struct perf_event *event = cpuc->events[idx];
 		struct hw_perf_event *hwc;
 
 		if (!test_bit(idx, cpuc->active_mask))
 			continue;
 
 		if (!xscale2_pmnc_counter_has_overflowed(pmnc, idx))
 			continue;
 
 		hwc = &event->hw;
 		armpmu_event_update(event, hwc, idx, 1);
 		data.period = event->hw.last_period;
  		if (!armpmu_event_set_period(event, hwc, idx))
  			continue;
  
		if (perf_event_overflow(event, 0, &data, regs))
// 		if (perf_event_overflow(event, &data, regs))
  			armpmu->disable(hwc, idx);
  	}
  
 	irq_work_run();
 
 	 
 	pmnc = xscale2pmu_read_pmnc() | XSCALE_PMU_ENABLE;
 	xscale2pmu_write_pmnc(pmnc);
 
 	return IRQ_HANDLED;
 }