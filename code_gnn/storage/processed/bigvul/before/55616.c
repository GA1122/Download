void sched_move_task(struct task_struct *tsk)
{
	struct task_group *tg;
	int queued, running;
	struct rq_flags rf;
	struct rq *rq;

	rq = task_rq_lock(tsk, &rf);

	running = task_current(rq, tsk);
	queued = task_on_rq_queued(tsk);

	if (queued)
		dequeue_task(rq, tsk, DEQUEUE_SAVE | DEQUEUE_MOVE);
	if (unlikely(running))
		put_prev_task(rq, tsk);

	 
	tg = container_of(task_css_check(tsk, cpu_cgrp_id, true),
			  struct task_group, css);
	tg = autogroup_task_group(tsk, tg);
	tsk->sched_task_group = tg;

#ifdef CONFIG_FAIR_GROUP_SCHED
	if (tsk->sched_class->task_move_group)
		tsk->sched_class->task_move_group(tsk);
	else
#endif
		set_task_rq(tsk, task_cpu(tsk));

	if (unlikely(running))
		tsk->sched_class->set_curr_task(rq);
	if (queued)
		enqueue_task(rq, tsk, ENQUEUE_RESTORE | ENQUEUE_MOVE);

	task_rq_unlock(rq, tsk, &rf);
}