static void sched_ttwu_pending(void)
{
	struct rq *rq = this_rq();
	struct task_struct *list = xchg(&rq->wake_list, NULL);

	if (!list)
		return;

	raw_spin_lock(&rq->lock);

	while (list) {
		struct task_struct *p = list;
		list = list->wake_entry;
		ttwu_do_activate(rq, p, 0);
	}

	raw_spin_unlock(&rq->lock);
}
