static void migrate_tasks(struct rq *dead_rq)
{
	struct rq *rq = dead_rq;
	struct task_struct *next, *stop = rq->stop;
	struct pin_cookie cookie;
	int dest_cpu;

	 
	rq->stop = NULL;

	 
	update_rq_clock(rq);

	for (;;) {
		 
		if (rq->nr_running == 1)
			break;

		 
		cookie = lockdep_pin_lock(&rq->lock);
		next = pick_next_task(rq, &fake_task, cookie);
		BUG_ON(!next);
		next->sched_class->put_prev_task(rq, next);

		 
		lockdep_unpin_lock(&rq->lock, cookie);
		raw_spin_unlock(&rq->lock);
		raw_spin_lock(&next->pi_lock);
		raw_spin_lock(&rq->lock);

		 
		if (WARN_ON(task_rq(next) != rq || !task_on_rq_queued(next))) {
			raw_spin_unlock(&next->pi_lock);
			continue;
		}

		 
		dest_cpu = select_fallback_rq(dead_rq->cpu, next);

		rq = __migrate_task(rq, next, dest_cpu);
		if (rq != dead_rq) {
			raw_spin_unlock(&rq->lock);
			rq = dead_rq;
			raw_spin_lock(&rq->lock);
		}
		raw_spin_unlock(&next->pi_lock);
	}

	rq->stop = stop;
}
