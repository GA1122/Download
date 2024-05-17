static int mipspmu_event_init(struct perf_event *event)
{
	int err = 0;

	switch (event->attr.type) {
	case PERF_TYPE_RAW:
	case PERF_TYPE_HARDWARE:
	case PERF_TYPE_HW_CACHE:
		break;

	default:
		return -ENOENT;
	}

	if (!mipspmu || event->cpu >= nr_cpumask_bits ||
		(event->cpu >= 0 && !cpu_online(event->cpu)))
		return -ENODEV;

	if (!atomic_inc_not_zero(&active_events)) {
		if (atomic_read(&active_events) > MIPS_MAX_HWEVENTS) {
			atomic_dec(&active_events);
			return -ENOSPC;
		}

		mutex_lock(&pmu_reserve_mutex);
		if (atomic_read(&active_events) == 0)
			err = mipspmu_get_irq();

		if (!err)
			atomic_inc(&active_events);
		mutex_unlock(&pmu_reserve_mutex);
	}

	if (err)
		return err;

	err = __hw_perf_event_init(event);
	if (err)
		hw_perf_event_destroy(event);

	return err;
}
