void thread_group_cputimer(struct task_struct *tsk, struct task_cputime *times)
{
	struct thread_group_cputimer *cputimer = &tsk->signal->cputimer;
	struct task_cputime sum;

	 
	if (!READ_ONCE(cputimer->running)) {
		 
		thread_group_cputime(tsk, &sum);
		update_gt_cputime(&cputimer->cputime_atomic, &sum);

		 
		WRITE_ONCE(cputimer->running, true);
	}
	sample_cputime_atomic(times, &cputimer->cputime_atomic);
}
