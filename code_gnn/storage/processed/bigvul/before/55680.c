static void ttwu_queue(struct task_struct *p, int cpu, int wake_flags)
{
	struct rq *rq = cpu_rq(cpu);
	struct pin_cookie cookie;

#if defined(CONFIG_SMP)
	if (sched_feat(TTWU_QUEUE) && !cpus_share_cache(smp_processor_id(), cpu)) {
		sched_clock_cpu(cpu);  
		ttwu_queue_remote(p, cpu, wake_flags);
		return;
	}
#endif

	raw_spin_lock(&rq->lock);
	cookie = lockdep_pin_lock(&rq->lock);
	ttwu_do_activate(rq, p, wake_flags, cookie);
	lockdep_unpin_lock(&rq->lock, cookie);
	raw_spin_unlock(&rq->lock);
}