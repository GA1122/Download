void move_task_off_dead_cpu(int dead_cpu, struct task_struct *p)
{
	struct rq *rq = cpu_rq(dead_cpu);
	int needs_cpu, uninitialized_var(dest_cpu);
	unsigned long flags;

	local_irq_save(flags);

	raw_spin_lock(&rq->lock);
	needs_cpu = (task_cpu(p) == dead_cpu) && (p->state != TASK_WAKING);
	if (needs_cpu)
		dest_cpu = select_fallback_rq(dead_cpu, p);
	raw_spin_unlock(&rq->lock);
	 
	if (needs_cpu)
		__migrate_task(p, dead_cpu, dest_cpu);
	local_irq_restore(flags);
}