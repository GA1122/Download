void sched_idle_next(void)
{
	int this_cpu = smp_processor_id();
	struct rq *rq = cpu_rq(this_cpu);
	struct task_struct *p = rq->idle;
	unsigned long flags;

	 
	BUG_ON(cpu_online(this_cpu));

	 
	raw_spin_lock_irqsave(&rq->lock, flags);

	__setscheduler(rq, p, SCHED_FIFO, MAX_RT_PRIO-1);

	activate_task(rq, p, 0);

	raw_spin_unlock_irqrestore(&rq->lock, flags);
}
