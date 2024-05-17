static inline void perf_cgroup_sched_in(struct task_struct *prev,
					struct task_struct *task)
{
	struct perf_cgroup *cgrp1;
	struct perf_cgroup *cgrp2 = NULL;

	 
	cgrp1 = perf_cgroup_from_task(task);

	 
	cgrp2 = perf_cgroup_from_task(prev);

	 
	if (cgrp1 != cgrp2)
		perf_cgroup_switch(task, PERF_CGROUP_SWIN);
}
