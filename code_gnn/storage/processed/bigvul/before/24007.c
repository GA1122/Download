static int airo_thread(void *data) {
	struct net_device *dev = data;
	struct airo_info *ai = dev->ml_priv;
	int locked;

	set_freezable();
	while(1) {
		 
		try_to_freeze();

		if (test_bit(JOB_DIE, &ai->jobs))
			break;

		if (ai->jobs) {
			locked = down_interruptible(&ai->sem);
		} else {
			wait_queue_t wait;

			init_waitqueue_entry(&wait, current);
			add_wait_queue(&ai->thr_wait, &wait);
			for (;;) {
				set_current_state(TASK_INTERRUPTIBLE);
				if (ai->jobs)
					break;
				if (ai->expires || ai->scan_timeout) {
					if (ai->scan_timeout &&
							time_after_eq(jiffies,ai->scan_timeout)){
						set_bit(JOB_SCAN_RESULTS, &ai->jobs);
						break;
					} else if (ai->expires &&
							time_after_eq(jiffies,ai->expires)){
						set_bit(JOB_AUTOWEP, &ai->jobs);
						break;
					}
					if (!kthread_should_stop() &&
					    !freezing(current)) {
						unsigned long wake_at;
						if (!ai->expires || !ai->scan_timeout) {
							wake_at = max(ai->expires,
								ai->scan_timeout);
						} else {
							wake_at = min(ai->expires,
								ai->scan_timeout);
						}
						schedule_timeout(wake_at - jiffies);
						continue;
					}
				} else if (!kthread_should_stop() &&
					   !freezing(current)) {
					schedule();
					continue;
				}
				break;
			}
			current->state = TASK_RUNNING;
			remove_wait_queue(&ai->thr_wait, &wait);
			locked = 1;
		}

		if (locked)
			continue;

		if (test_bit(JOB_DIE, &ai->jobs)) {
			up(&ai->sem);
			break;
		}

		if (ai->power.event || test_bit(FLAG_FLASHING, &ai->flags)) {
			up(&ai->sem);
			continue;
		}

		if (test_bit(JOB_XMIT, &ai->jobs))
			airo_end_xmit(dev);
		else if (test_bit(JOB_XMIT11, &ai->jobs))
			airo_end_xmit11(dev);
		else if (test_bit(JOB_STATS, &ai->jobs))
			airo_read_stats(dev);
		else if (test_bit(JOB_WSTATS, &ai->jobs))
			airo_read_wireless_stats(ai);
		else if (test_bit(JOB_PROMISC, &ai->jobs))
			airo_set_promisc(ai);
		else if (test_bit(JOB_MIC, &ai->jobs))
			micinit(ai);
		else if (test_bit(JOB_EVENT, &ai->jobs))
			airo_send_event(dev);
		else if (test_bit(JOB_AUTOWEP, &ai->jobs))
			timer_func(dev);
		else if (test_bit(JOB_SCAN_RESULTS, &ai->jobs))
			airo_process_scan_results(ai);
		else   
			up(&ai->sem);
	}

	return 0;
}