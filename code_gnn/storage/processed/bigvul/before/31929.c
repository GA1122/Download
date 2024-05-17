static inline void perf_cgroup_sched_out(struct task_struct *task,
					 struct task_struct *next)
{
	struct perf_cgroup *cgrp1;
	struct perf_cgroup *cgrp2 = NULL;

	 
	cgrp1 = perf_cgroup_from_task(task);

	 
	if (next)
		cgrp2 = perf_cgroup_from_task(next);

	 
	if (cgrp1 != cgrp2)
		perf_cgroup_switch(task, PERF_CGROUP_SWOUT);
}
