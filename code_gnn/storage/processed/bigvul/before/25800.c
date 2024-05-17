static int x86_schedule_events(struct cpu_hw_events *cpuc, int n, int *assign)
{
	struct event_constraint *c, *constraints[X86_PMC_IDX_MAX];
	unsigned long used_mask[BITS_TO_LONGS(X86_PMC_IDX_MAX)];
	int i, j, w, wmax, num = 0;
	struct hw_perf_event *hwc;

	bitmap_zero(used_mask, X86_PMC_IDX_MAX);

	for (i = 0; i < n; i++) {
		c = x86_pmu.get_event_constraints(cpuc, cpuc->event_list[i]);
		constraints[i] = c;
	}

	 
	for (i = 0; i < n; i++) {
		hwc = &cpuc->event_list[i]->hw;
		c = constraints[i];

		 
		if (hwc->idx == -1)
			break;

		 
		if (!test_bit(hwc->idx, c->idxmsk))
			break;

		 
		if (test_bit(hwc->idx, used_mask))
			break;

		__set_bit(hwc->idx, used_mask);
		if (assign)
			assign[i] = hwc->idx;
	}
	if (i == n)
		goto done;

	 

	bitmap_zero(used_mask, X86_PMC_IDX_MAX);

	 
	wmax = x86_pmu.num_counters;

	 
	if (x86_pmu.num_counters_fixed)
		wmax++;

	for (w = 1, num = n; num && w <= wmax; w++) {
		 
		for (i = 0; num && i < n; i++) {
			c = constraints[i];
			hwc = &cpuc->event_list[i]->hw;

			if (c->weight != w)
				continue;

			for_each_set_bit(j, c->idxmsk, X86_PMC_IDX_MAX) {
				if (!test_bit(j, used_mask))
					break;
			}

			if (j == X86_PMC_IDX_MAX)
				break;

			__set_bit(j, used_mask);

			if (assign)
				assign[i] = j;
			num--;
		}
	}
done:
	 
	if (!assign || num) {
		for (i = 0; i < n; i++) {
			if (x86_pmu.put_event_constraints)
				x86_pmu.put_event_constraints(cpuc, cpuc->event_list[i]);
		}
	}
	return num ? -ENOSPC : 0;
}
