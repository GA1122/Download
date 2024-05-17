static void migrate_dead(unsigned int dead_cpu, struct task_struct *p)
{
	struct rq *rq = cpu_rq(dead_cpu);

	 
	BUG_ON(!p->exit_state);

	 
	BUG_ON(p->state == TASK_DEAD);

	get_task_struct(p);

	 
	raw_spin_unlock_irq(&rq->lock);
	move_task_off_dead_cpu(dead_cpu, p);
	raw_spin_lock_irq(&rq->lock);

	put_task_struct(p);
}
