static int intel_pmu_handle_irq(struct pt_regs *regs)
{
	struct perf_sample_data data;
	struct cpu_hw_events *cpuc;
	int bit, loops;
	u64 status;
	int handled;

	cpuc = &__get_cpu_var(cpu_hw_events);

	 
	apic_write(APIC_LVTPC, APIC_DM_NMI);

	intel_pmu_disable_all();
	handled = intel_pmu_drain_bts_buffer();
	status = intel_pmu_get_status();
	if (!status) {
		intel_pmu_enable_all(0);
		return handled;
	}

	loops = 0;
again:
	intel_pmu_ack_status(status);
	if (++loops > 100) {
		WARN_ONCE(1, "perfevents: irq loop stuck!\n");
		perf_event_print_debug();
		intel_pmu_reset();
		goto done;
	}

	inc_irq_stat(apic_perf_irqs);

	intel_pmu_lbr_read();

	 
	if (__test_and_clear_bit(62, (unsigned long *)&status)) {
		handled++;
		x86_pmu.drain_pebs(regs);
	}

	for_each_set_bit(bit, (unsigned long *)&status, X86_PMC_IDX_MAX) {
		struct perf_event *event = cpuc->events[bit];

		handled++;

		if (!test_bit(bit, cpuc->active_mask))
			continue;

		if (!intel_pmu_save_and_restart(event))
			continue;

		perf_sample_data_init(&data, 0, event->hw.last_period);

		if (has_branch_stack(event))
			data.br_stack = &cpuc->lbr_stack;

		if (perf_event_overflow(event, &data, regs))
			x86_pmu_stop(event, 0);
	}

	 
	status = intel_pmu_get_status();
	if (status)
		goto again;

done:
	intel_pmu_enable_all(0);
	return handled;
}
