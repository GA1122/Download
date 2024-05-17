list_update_cgroup_event(struct perf_event *event,
			 struct perf_event_context *ctx, bool add)
{
	struct perf_cpu_context *cpuctx;

	if (!is_cgroup_event(event))
		return;

	if (add && ctx->nr_cgroups++)
		return;
	else if (!add && --ctx->nr_cgroups)
		return;
	 
	cpuctx = __get_cpu_context(ctx);

	 
	if (add && perf_cgroup_from_task(current, ctx) == event->cgrp)
		cpuctx->cgrp = event->cgrp;
	else if (!add)
		cpuctx->cgrp = NULL;
}
