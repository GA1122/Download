static void cpu_timer_fire(struct k_itimer *timer)
{
	if ((timer->it_sigev_notify & ~SIGEV_THREAD_ID) == SIGEV_NONE) {
		 
		timer->it.cpu.expires = 0;
	} else if (unlikely(timer->sigq == NULL)) {
		 
		wake_up_process(timer->it_process);
		timer->it.cpu.expires = 0;
	} else if (timer->it.cpu.incr == 0) {
		 
		posix_timer_event(timer, 0);
		timer->it.cpu.expires = 0;
	} else if (posix_timer_event(timer, ++timer->it_requeue_pending)) {
		 
		posix_cpu_timer_rearm(timer);
		++timer->it_requeue_pending;
	}
}
