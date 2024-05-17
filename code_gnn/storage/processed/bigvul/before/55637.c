void scheduler_ipi(void)
{
	 
	preempt_fold_need_resched();

	if (llist_empty(&this_rq()->wake_list) && !got_nohz_idle_kick())
		return;

	 
	irq_enter();
	sched_ttwu_pending();

	 
	if (unlikely(got_nohz_idle_kick())) {
		this_rq()->idle_balance = 1;
		raise_softirq_irqoff(SCHED_SOFTIRQ);
	}
	irq_exit();
}