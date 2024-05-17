static inline void sched_submit_work(struct task_struct *tsk)
{
	if (!tsk->state || tsk_is_pi_blocked(tsk))
		return;
	 
	if (blk_needs_flush_plug(tsk))
		blk_schedule_flush_plug(tsk);
}
