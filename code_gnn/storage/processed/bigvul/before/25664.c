static void sparc_pmu_del(struct perf_event *event, int _flags)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	unsigned long flags;
	int i;

	local_irq_save(flags);
	perf_pmu_disable(event->pmu);

	for (i = 0; i < cpuc->n_events; i++) {
		if (event == cpuc->event[i]) {
			 
			sparc_pmu_stop(event, PERF_EF_UPDATE);

			 
			while (++i < cpuc->n_events) {
				cpuc->event[i - 1] = cpuc->event[i];
				cpuc->events[i - 1] = cpuc->events[i];
				cpuc->current_idx[i - 1] =
					cpuc->current_idx[i];
			}

			perf_event_update_userpage(event);

			cpuc->n_events--;
			break;
		}
	}

	perf_pmu_enable(event->pmu);
	local_irq_restore(flags);
}
