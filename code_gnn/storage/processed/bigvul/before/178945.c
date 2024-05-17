 armv6pmu_handle_irq(int irq_num,
 		    void *dev)
 {
 	unsigned long pmcr = armv6_pmcr_read();
 	struct perf_sample_data data;
 	struct cpu_hw_events *cpuc;
 	struct pt_regs *regs;
 	int idx;
 
 	if (!armv6_pmcr_has_overflowed(pmcr))
 		return IRQ_NONE;
 
 	regs = get_irq_regs();
 
 	 
 	armv6_pmcr_write(pmcr);
 
 	perf_sample_data_init(&data, 0);
 
 	cpuc = &__get_cpu_var(cpu_hw_events);
 	for (idx = 0; idx <= armpmu->num_events; ++idx) {
 		struct perf_event *event = cpuc->events[idx];
 		struct hw_perf_event *hwc;
 
 		if (!test_bit(idx, cpuc->active_mask))
 			continue;
 
 		 
 		if (!armv6_pmcr_counter_has_overflowed(pmcr, idx))
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
 
 	return IRQ_HANDLED;
 }