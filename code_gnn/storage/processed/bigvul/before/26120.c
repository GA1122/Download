void perf_event_wakeup(struct perf_event *event)
{
	wake_up_all(&event->waitq);

	if (event->pending_kill) {
		kill_fasync(&event->fasync, SIGIO, event->pending_kill);
		event->pending_kill = 0;
	}
}
