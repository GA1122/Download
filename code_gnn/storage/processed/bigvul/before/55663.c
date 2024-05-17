int task_can_attach(struct task_struct *p,
		    const struct cpumask *cs_cpus_allowed)
{
	int ret = 0;

	 
	if (p->flags & PF_NO_SETAFFINITY) {
		ret = -EINVAL;
		goto out;
	}

#ifdef CONFIG_SMP
	if (dl_task(p) && !cpumask_intersects(task_rq(p)->rd->span,
					      cs_cpus_allowed)) {
		unsigned int dest_cpu = cpumask_any_and(cpu_active_mask,
							cs_cpus_allowed);
		struct dl_bw *dl_b;
		bool overflow;
		int cpus;
		unsigned long flags;

		rcu_read_lock_sched();
		dl_b = dl_bw_of(dest_cpu);
		raw_spin_lock_irqsave(&dl_b->lock, flags);
		cpus = dl_bw_cpus(dest_cpu);
		overflow = __dl_overflow(dl_b, cpus, 0, p->dl.dl_bw);
		if (overflow)
			ret = -EBUSY;
		else {
			 
			__dl_add(dl_b, p->dl.dl_bw);
		}
		raw_spin_unlock_irqrestore(&dl_b->lock, flags);
		rcu_read_unlock_sched();

	}
#endif
out:
	return ret;
}