static void perf_event_exit_cpu(int cpu)
{
	struct swevent_htable *swhash = &per_cpu(swevent_htable, cpu);

	mutex_lock(&swhash->hlist_mutex);
	swevent_hlist_release(swhash);
	mutex_unlock(&swhash->hlist_mutex);

	perf_event_exit_cpu_context(cpu);
}
