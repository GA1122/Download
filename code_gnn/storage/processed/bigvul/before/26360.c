static int ttwu_activate_remote(struct task_struct *p, int wake_flags)
{
	struct rq *rq;
	int ret = 0;

	rq = __task_rq_lock(p);
	if (p->on_cpu) {
		ttwu_activate(rq, p, ENQUEUE_WAKEUP);
		ttwu_do_wakeup(rq, p, wake_flags);
		ret = 1;
	}
	__task_rq_unlock(rq);

	return ret;

}
