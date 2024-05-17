perf_cgroup_set_timestamp(struct task_struct *task,
			  struct perf_event_context *ctx)
{
	struct perf_cgroup *cgrp;
	struct perf_cgroup_info *info;

	 
	if (!task || !ctx->nr_cgroups)
		return;

	cgrp = perf_cgroup_from_task(task);
	info = this_cpu_ptr(cgrp->info);
	info->timestamp = ctx->timestamp;
}
