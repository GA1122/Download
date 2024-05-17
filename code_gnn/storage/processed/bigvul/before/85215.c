static void perf_cgroup_switch(struct task_struct *task, int mode)
{
	struct perf_cpu_context *cpuctx;
	struct list_head *list;
	unsigned long flags;

	 
	local_irq_save(flags);

	list = this_cpu_ptr(&cgrp_cpuctx_list);
	list_for_each_entry(cpuctx, list, cgrp_cpuctx_entry) {
		WARN_ON_ONCE(cpuctx->ctx.nr_cgroups == 0);

		perf_ctx_lock(cpuctx, cpuctx->task_ctx);
		perf_pmu_disable(cpuctx->ctx.pmu);

		if (mode & PERF_CGROUP_SWOUT) {
			cpu_ctx_sched_out(cpuctx, EVENT_ALL);
			 
			cpuctx->cgrp = NULL;
		}

		if (mode & PERF_CGROUP_SWIN) {
			WARN_ON_ONCE(cpuctx->cgrp);
			 
			cpuctx->cgrp = perf_cgroup_from_task(task,
							     &cpuctx->ctx);
			cpu_ctx_sched_in(cpuctx, EVENT_ALL, task);
		}
		perf_pmu_enable(cpuctx->ctx.pmu);
		perf_ctx_unlock(cpuctx, cpuctx->task_ctx);
	}

	local_irq_restore(flags);
}
