static void ttwu_queue(struct task_struct *p, int cpu)
{
	struct rq *rq = cpu_rq(cpu);

#if defined(CONFIG_SMP)
	if (sched_feat(TTWU_QUEUE) && cpu != smp_processor_id()) {
		sched_clock_cpu(cpu);  
		ttwu_queue_remote(p, cpu);
		return;
	}
#endif

	raw_spin_lock(&rq->lock);
	ttwu_do_activate(rq, p, 0);
	raw_spin_unlock(&rq->lock);
}