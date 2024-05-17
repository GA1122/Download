static int __set_cpus_allowed_ptr(struct task_struct *p,
				  const struct cpumask *new_mask, bool check)
{
	const struct cpumask *cpu_valid_mask = cpu_active_mask;
	unsigned int dest_cpu;
	struct rq_flags rf;
	struct rq *rq;
	int ret = 0;

	rq = task_rq_lock(p, &rf);

	if (p->flags & PF_KTHREAD) {
		 
		cpu_valid_mask = cpu_online_mask;
	}

	 
	if (check && (p->flags & PF_NO_SETAFFINITY)) {
		ret = -EINVAL;
		goto out;
	}

	if (cpumask_equal(&p->cpus_allowed, new_mask))
		goto out;

	if (!cpumask_intersects(new_mask, cpu_valid_mask)) {
		ret = -EINVAL;
		goto out;
	}

	do_set_cpus_allowed(p, new_mask);

	if (p->flags & PF_KTHREAD) {
		 
		WARN_ON(cpumask_intersects(new_mask, cpu_online_mask) &&
			!cpumask_intersects(new_mask, cpu_active_mask) &&
			p->nr_cpus_allowed != 1);
	}

	 
	if (cpumask_test_cpu(task_cpu(p), new_mask))
		goto out;

	dest_cpu = cpumask_any_and(cpu_valid_mask, new_mask);
	if (task_running(rq, p) || p->state == TASK_WAKING) {
		struct migration_arg arg = { p, dest_cpu };
		 
		task_rq_unlock(rq, p, &rf);
		stop_one_cpu(cpu_of(rq), migration_cpu_stop, &arg);
		tlb_migrate_finish(p->mm);
		return 0;
	} else if (task_on_rq_queued(p)) {
		 
		lockdep_unpin_lock(&rq->lock, rf.cookie);
		rq = move_queued_task(rq, p, dest_cpu);
		lockdep_repin_lock(&rq->lock, rf.cookie);
	}
out:
	task_rq_unlock(rq, p, &rf);

	return ret;
}
