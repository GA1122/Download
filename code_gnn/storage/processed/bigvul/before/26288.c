void __cpuinit init_idle(struct task_struct *idle, int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long flags;

	raw_spin_lock_irqsave(&rq->lock, flags);

	__sched_fork(idle);
	idle->state = TASK_RUNNING;
	idle->se.exec_start = sched_clock();

	do_set_cpus_allowed(idle, cpumask_of(cpu));
	 
	rcu_read_lock();
	__set_task_cpu(idle, cpu);
	rcu_read_unlock();

	rq->curr = rq->idle = idle;
#if defined(CONFIG_SMP)
	idle->on_cpu = 1;
#endif
	raw_spin_unlock_irqrestore(&rq->lock, flags);

	 
	task_thread_info(idle)->preempt_count = 0;

	 
	idle->sched_class = &idle_sched_class;
	ftrace_graph_init_idle_task(idle, cpu);
}
