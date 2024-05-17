pick_next_task(struct rq *rq, struct task_struct *prev, struct pin_cookie cookie)
{
	const struct sched_class *class = &fair_sched_class;
	struct task_struct *p;

	 
	if (likely(prev->sched_class == class &&
		   rq->nr_running == rq->cfs.h_nr_running)) {
		p = fair_sched_class.pick_next_task(rq, prev, cookie);
		if (unlikely(p == RETRY_TASK))
			goto again;

		 
		if (unlikely(!p))
			p = idle_sched_class.pick_next_task(rq, prev, cookie);

		return p;
	}

again:
	for_each_class(class) {
		p = class->pick_next_task(rq, prev, cookie);
		if (p) {
			if (unlikely(p == RETRY_TASK))
				goto again;
			return p;
		}
	}

	BUG();  
}
