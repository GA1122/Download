u64 perf_event_read_local(struct perf_event *event)
{
	unsigned long flags;
	u64 val;

	 
	local_irq_save(flags);

	 
	WARN_ON_ONCE((event->attach_state & PERF_ATTACH_TASK) &&
		     event->hw.target != current);

	 
	WARN_ON_ONCE(!(event->attach_state & PERF_ATTACH_TASK) &&
		     event->cpu != smp_processor_id());

	 
	WARN_ON_ONCE(event->attr.inherit);

	 
	WARN_ON_ONCE(event->pmu->count);

	 
	if (event->oncpu == smp_processor_id())
		event->pmu->read(event);

	val = local64_read(&event->count);
	local_irq_restore(flags);

	return val;
}
