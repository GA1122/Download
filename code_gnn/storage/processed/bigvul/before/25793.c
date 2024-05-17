static int x86_pmu_hw_config(struct perf_event *event)
{
	if (event->attr.precise_ip) {
		int precise = 0;

		 
		if (x86_pmu.pebs_active) {
			precise++;

			 
			if (x86_pmu.lbr_nr)
				precise++;
		}

		if (event->attr.precise_ip > precise)
			return -EOPNOTSUPP;
	}

	 
	event->hw.config = ARCH_PERFMON_EVENTSEL_INT;

	 
	if (!event->attr.exclude_user)
		event->hw.config |= ARCH_PERFMON_EVENTSEL_USR;
	if (!event->attr.exclude_kernel)
		event->hw.config |= ARCH_PERFMON_EVENTSEL_OS;

	if (event->attr.type == PERF_TYPE_RAW)
		event->hw.config |= event->attr.config & X86_RAW_EVENT_MASK;

	return x86_setup_perfctr(event);
}