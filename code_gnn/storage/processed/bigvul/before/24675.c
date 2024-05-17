static void cpu_timer_fire(struct k_itimer *timer)
{
	if (unlikely(timer->sigq == NULL)) {
		 
		wake_up_process(timer->it_process);
		timer->it.cpu.expires.sched = 0;
	} else if (timer->it.cpu.incr.sched == 0) {
		 
		posix_timer_event(timer, 0);
		timer->it.cpu.expires.sched = 0;
	} else if (posix_timer_event(timer, ++timer->it_requeue_pending)) {
		 
		posix_cpu_timer_schedule(timer);
	}
}
