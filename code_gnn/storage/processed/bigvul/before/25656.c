void perf_event_release_pmc(void)
{
	if (atomic_dec_and_mutex_lock(&active_events, &pmc_grab_mutex)) {
		if (atomic_read(&nmi_active) == 0)
			on_each_cpu(start_nmi_watchdog, NULL, 1);
		mutex_unlock(&pmc_grab_mutex);
	}
}
