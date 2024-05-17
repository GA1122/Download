static u64 maybe_change_configuration(struct cpu_hw_events *cpuc, u64 pcr)
{
	int i;

	if (!cpuc->n_added)
		goto out;

	 
	for (i = 0; i < cpuc->n_events; i++) {
		struct perf_event *cp = cpuc->event[i];

		if (cpuc->current_idx[i] != PIC_NO_INDEX &&
		    cpuc->current_idx[i] != cp->hw.idx) {
			sparc_perf_event_update(cp, &cp->hw,
						cpuc->current_idx[i]);
			cpuc->current_idx[i] = PIC_NO_INDEX;
		}
	}

	 
	for (i = 0; i < cpuc->n_events; i++) {
		struct perf_event *cp = cpuc->event[i];
		struct hw_perf_event *hwc = &cp->hw;
		int idx = hwc->idx;
		u64 enc;

		if (cpuc->current_idx[i] != PIC_NO_INDEX)
			continue;

		sparc_perf_event_set_period(cp, hwc, idx);
		cpuc->current_idx[i] = idx;

		enc = perf_event_get_enc(cpuc->events[i]);
		pcr &= ~mask_for_index(idx);
		if (hwc->state & PERF_HES_STOPPED)
			pcr |= nop_for_index(idx);
		else
			pcr |= event_encoding(enc, idx);
	}
out:
	return pcr;
}
