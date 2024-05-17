static inline void update_cgrp_time_from_event(struct perf_event *event)
{
	struct perf_cgroup *cgrp;

	 
	if (!is_cgroup_event(event))
		return;

	cgrp = perf_cgroup_from_task(current);
	 
	if (cgrp == event->cgrp)
		__update_cgrp_time(event->cgrp);
}
