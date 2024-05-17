void perf_cgroup_switch(struct task_struct *task, int mode)
{
	struct perf_cpu_context *cpuctx;
	struct pmu *pmu;
	unsigned long flags;

	 
	local_irq_save(flags);

	 
	rcu_read_lock();

	list_for_each_entry_rcu(pmu, &pmus, entry) {
		cpuctx = this_cpu_ptr(pmu->pmu_cpu_context);

		 
		if (cpuctx->ctx.nr_cgroups > 0) {
			perf_ctx_lock(cpuctx, cpuctx->task_ctx);
			perf_pmu_disable(cpuctx->ctx.pmu);

			if (mode & PERF_CGROUP_SWOUT) {
				cpu_ctx_sched_out(cpuctx, EVENT_ALL);
				 
				cpuctx->cgrp = NULL;
			}

			if (mode & PERF_CGROUP_SWIN) {
				WARN_ON_ONCE(cpuctx->cgrp);
				 
				cpuctx->cgrp = perf_cgroup_from_task(task);
				cpu_ctx_sched_in(cpuctx, EVENT_ALL, task);
			}
			perf_pmu_enable(cpuctx->ctx.pmu);
			perf_ctx_unlock(cpuctx, cpuctx->task_ctx);
		}
	}

	rcu_read_unlock();

	local_irq_restore(flags);
}
