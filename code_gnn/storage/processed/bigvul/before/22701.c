static void power_pmu_read(struct perf_event *event)
{
	s64 val, delta, prev;

	if (event->hw.state & PERF_HES_STOPPED)
		return;

	if (!event->hw.idx)
		return;
	 
	do {
		prev = local64_read(&event->hw.prev_count);
		barrier();
		val = read_pmc(event->hw.idx);
	} while (local64_cmpxchg(&event->hw.prev_count, prev, val) != prev);

	 
	delta = (val - prev) & 0xfffffffful;
	local64_add(delta, &event->count);
	local64_sub(delta, &event->hw.period_left);
}
