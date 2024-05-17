static int ttwu_remote(struct task_struct *p, int wake_flags)
{
	struct rq_flags rf;
	struct rq *rq;
	int ret = 0;

	rq = __task_rq_lock(p, &rf);
	if (task_on_rq_queued(p)) {
		 
		update_rq_clock(rq);
		ttwu_do_wakeup(rq, p, wake_flags, rf.cookie);
		ret = 1;
	}
	__task_rq_unlock(rq, &rf);

	return ret;
}