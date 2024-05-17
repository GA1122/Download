perf_cgroup_set_shadow_time(struct perf_event *event, u64 now)
{
	struct perf_cgroup_info *t;
	t = per_cpu_ptr(event->cgrp->info, event->cpu);
	event->shadow_ctx_time = now - t->timestamp;
}
