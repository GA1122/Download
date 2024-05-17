static int alpha_pmu_event_init(struct perf_event *event)
{
	int err;

	switch (event->attr.type) {
	case PERF_TYPE_RAW:
	case PERF_TYPE_HARDWARE:
	case PERF_TYPE_HW_CACHE:
		break;

	default:
		return -ENOENT;
	}

	if (!alpha_pmu)
		return -ENODEV;

	 
	err = __hw_perf_event_init(event);

	return err;
}
